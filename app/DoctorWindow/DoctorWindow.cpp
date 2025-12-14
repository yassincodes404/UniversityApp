#include "DoctorWindow.h"
#include "../DAL/DoctorRepo.h"
#include "../DAL/CourseRepo.h"
#include "../DAL/DatabaseManager.h"
#include <QMenuBar>
#include <QLineEdit>
#include <QMenu>
#include <QLabel>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QTabWidget>
#include <QComboBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QHeaderView>
#include <QInputDialog>

DoctorWindow::DoctorWindow(int doctorId, QWidget *parent)
    : QMainWindow(parent)
    , m_doctorId(doctorId)
{
    setupUI();
    setupMenuBar();
    loadCourses();
    setWindowTitle("Doctor Portal - NCTU SIS");
    resize(1000, 700);
}

DoctorWindow::~DoctorWindow()
{
}

void DoctorWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Logout button
    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addStretch();
    m_logoutButton = new QPushButton("Logout", this);
    connect(m_logoutButton, &QPushButton::clicked, this, &DoctorWindow::onLogout);
    topLayout->addWidget(m_logoutButton);
    mainLayout->addLayout(topLayout);
    
    // Course selection
    QHBoxLayout* courseLayout = new QHBoxLayout;
    courseLayout->addWidget(new QLabel("Select Course:", this));
    m_courseCombo = new QComboBox(this);
    connect(m_courseCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DoctorWindow::onCourseSelected);
    courseLayout->addWidget(m_courseCombo);
    courseLayout->addStretch();
    mainLayout->addLayout(courseLayout);
    
    // Tabs
    m_tabs = new QTabWidget(this);
    
    // Students tab
    QWidget* studentsWidget = new QWidget;
    QVBoxLayout* studentsLayout = new QVBoxLayout(studentsWidget);
    
    QPushButton* refreshBtn = new QPushButton("Refresh", this);
    connect(refreshBtn, &QPushButton::clicked, this, &DoctorWindow::refreshData);

    // Student search
    QHBoxLayout* studentSearchLayout = new QHBoxLayout;
    m_studentSearchEdit = new QLineEdit(this);
    m_studentSearchEdit->setPlaceholderText("Search students by code, name, or email...");
    connect(m_studentSearchEdit, &QLineEdit::textChanged,
            this, &DoctorWindow::loadStudents);
    studentSearchLayout->addWidget(m_studentSearchEdit);
    
    m_studentsTable = new QTableView(this);
    QSqlQueryModel* studentsModel = new QSqlQueryModel(m_studentsTable);
    m_studentsTable->setModel(studentsModel);
    m_studentsTable->horizontalHeader()->setStretchLastSection(true);
    
    studentsLayout->addWidget(refreshBtn);
    studentsLayout->addLayout(studentSearchLayout);
    studentsLayout->addWidget(m_studentsTable);
    m_tabs->addTab(studentsWidget, "Students");
    
    // Grades tab
    QWidget* gradesWidget = new QWidget;
    QVBoxLayout* gradesLayout = new QVBoxLayout(gradesWidget);
    
    QHBoxLayout* gradesButtonLayout = new QHBoxLayout;
    QPushButton* enterGradesBtn = new QPushButton("Enter Grades", this);
    connect(enterGradesBtn, &QPushButton::clicked, this, &DoctorWindow::onEnterGrades);
    gradesButtonLayout->addWidget(enterGradesBtn);
    gradesButtonLayout->addStretch();

    // Grade search
    QHBoxLayout* gradeSearchLayout = new QHBoxLayout;
    m_gradeSearchEdit = new QLineEdit(this);
    m_gradeSearchEdit->setPlaceholderText("Search grades by student code or name...");
    connect(m_gradeSearchEdit, &QLineEdit::textChanged,
            this, &DoctorWindow::loadStudents);
    gradeSearchLayout->addWidget(m_gradeSearchEdit);
    
    m_gradesTable = new QTableView(this);
    QSqlQueryModel* gradesModel = new QSqlQueryModel(m_gradesTable);
    m_gradesTable->setModel(gradesModel);
    m_gradesTable->horizontalHeader()->setStretchLastSection(true);
    m_gradesTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_gradesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_gradesTable, &QTableView::doubleClicked,
            this, &DoctorWindow::onGradeCellDoubleClicked);
    
    gradesLayout->addLayout(gradesButtonLayout);
    gradesLayout->addLayout(gradeSearchLayout);
    gradesLayout->addWidget(m_gradesTable);
    m_tabs->addTab(gradesWidget, "Grades");
    
    mainLayout->addWidget(m_tabs);
    
    statusBar()->showMessage("Welcome, Doctor");
}

void DoctorWindow::setupMenuBar()
{
    QMenu* fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("Refresh", this, &DoctorWindow::refreshData);
    fileMenu->addSeparator();
    fileMenu->addAction("Logout", this, &DoctorWindow::onLogout);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close);
}

void DoctorWindow::onLogout()
{
    emit logoutRequested();
}

void DoctorWindow::onCourseSelected(int index)
{
    Q_UNUSED(index);
    loadStudents();
}

void DoctorWindow::onEnterGrades()
{
    QMessageBox::information(
        this,
        "Enter Grades",
        "To edit a student's marks, double-click on the desired mark cell in the Grades tab.\n"
        "After entering the new value, the database will be updated automatically."
    );
}

void DoctorWindow::onMarkAttendance()
{
    QMessageBox::information(this, "Mark Attendance", "Attendance marking functionality - to be implemented");
}

void DoctorWindow::onGradeCellDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    // Only allow editing of numeric mark columns
    int col = index.column();
    if (col < 2 || col > 6) { // columns: 2-mid1,3-mid2,4-final,5-practical,6-assignments
        return;
    }

    QSqlQueryModel* gradesModel = qobject_cast<QSqlQueryModel*>(m_gradesTable->model());
    if (!gradesModel) {
        return;
    }

    int row = index.row();

    // Map column index to database column name
    QString columnName;
    switch (col) {
    case 2: columnName = "midterm1"; break;
    case 3: columnName = "midterm2"; break;
    case 4: columnName = "final_exam"; break;
    case 5: columnName = "practical"; break;
    case 6: columnName = "assignments"; break;
    default:
        return;
    }

    QString studentCode = gradesModel->data(gradesModel->index(row, 0)).toString();
    QString studentName = gradesModel->data(gradesModel->index(row, 1)).toString();
    double currentValue = gradesModel->data(index).toDouble();

    bool ok = false;
    double newValue = QInputDialog::getDouble(
        this,
        "Edit Mark",
        QString("Enter new %1 mark for %2 (%3):").arg(columnName, studentName, studentCode),
        currentValue,
        0.0,
        1000.0,
        2,
        &ok
    );
    if (!ok) {
        return; // user cancelled
    }

    // Grade record id is stored in hidden column 10
    int gradeId = gradesModel->data(gradesModel->index(row, 10)).toInt();
    if (gradeId <= 0) {
        QMessageBox::warning(this, "Error", "Unable to identify grade record for update.");
        return;
    }

    QSqlQuery updateQuery(DatabaseManager::instance().database());
    updateQuery.prepare(
        QString(
            "UPDATE grades "
            "SET %1 = ?, "
            "    total = COALESCE(midterm1,0) + COALESCE(midterm2,0) + "
            "            COALESCE(final_exam,0) + COALESCE(practical,0) + COALESCE(assignments,0) "
            "WHERE id = ?"
        ).arg(columnName)
    );
    updateQuery.addBindValue(newValue);
    updateQuery.addBindValue(gradeId);

    if (!updateQuery.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to update grade in database.");
        return;
    }

    // Refresh grades to reflect the new values and recalculated total
    loadStudents();
}

void DoctorWindow::refreshData()
{
    loadCourses();
    loadStudents();
}

void DoctorWindow::loadCourses()
{
    m_courseCombo->clear();
    
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT DISTINCT c.id, c.code, c.name "
        "FROM sections s "
        "JOIN courses c ON s.course_id = c.id "
        "WHERE s.doctor_id = ? "
        "ORDER BY c.code"
    );
    query.addBindValue(m_doctorId);
    
    if (query.exec()) {
        while (query.next()) {
            int courseId = query.value(0).toInt();
            QString text = QString("%1 - %2").arg(query.value(1).toString()).arg(query.value(2).toString());
            m_courseCombo->addItem(text, courseId);
        }
    }
    
    if (m_courseCombo->count() > 0) {
        loadStudents();
    }
}

void DoctorWindow::loadStudents()
{
    if (m_courseCombo->currentIndex() < 0) {
        return;
    }
    
    int courseId = m_courseCombo->currentData().toInt();
    
    QString studentFilter = m_studentSearchEdit ? m_studentSearchEdit->text().trimmed() : QString();
    QString gradeFilter = m_gradeSearchEdit ? m_gradeSearchEdit->text().trimmed() : QString();
    
    // Load students enrolled in this course
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(m_studentsTable->model());
    if (model) {
        QSqlQuery query(DatabaseManager::instance().database());
        QString sql =
            "SELECT s.student_code, s.name, s.email, e.status "
            "FROM enrollments e "
            "JOIN students s ON e.student_id = s.id "
            "WHERE e.course_id = ? AND e.status = 'registered' ";
        if (!studentFilter.isEmpty()) {
            sql += "AND (s.student_code LIKE ? OR s.name LIKE ? OR s.email LIKE ?) ";
        }
        sql += "ORDER BY s.student_code";
        
        query.prepare(sql);
        query.addBindValue(courseId);
        if (!studentFilter.isEmpty()) {
            QString pattern = "%" + studentFilter + "%";
            query.addBindValue(pattern);
            query.addBindValue(pattern);
            query.addBindValue(pattern);
        }
        query.exec();
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, "Student Code");
        model->setHeaderData(1, Qt::Horizontal, "Name");
        model->setHeaderData(2, Qt::Horizontal, "Email");
        model->setHeaderData(3, Qt::Horizontal, "Status");
    }
    
    // Load grades for this course
    QSqlQueryModel* gradesModel = qobject_cast<QSqlQueryModel*>(m_gradesTable->model());
    if (gradesModel) {
        QSqlQuery gradesQuery(DatabaseManager::instance().database());
        QString sql =
            "SELECT s.student_code, s.name, g.midterm1, g.midterm2, g.final_exam, "
            "g.practical, g.assignments, g.total, g.letter_grade, g.is_published, g.id "
            "FROM grades g "
            "JOIN students s ON g.student_id = s.id "
            "WHERE g.course_id = ? ";
        if (!gradeFilter.isEmpty()) {
            sql += "AND (s.student_code LIKE ? OR s.name LIKE ?) ";
        }
        sql += "ORDER BY s.student_code";
        
        gradesQuery.prepare(sql);
        gradesQuery.addBindValue(courseId);
        if (!gradeFilter.isEmpty()) {
            QString pattern = "%" + gradeFilter + "%";
            gradesQuery.addBindValue(pattern);
            gradesQuery.addBindValue(pattern);
        }
        gradesQuery.exec();
        gradesModel->setQuery(gradesQuery);
        gradesModel->setHeaderData(0, Qt::Horizontal, "Student Code");
        gradesModel->setHeaderData(1, Qt::Horizontal, "Name");
        gradesModel->setHeaderData(2, Qt::Horizontal, "Midterm 1");
        gradesModel->setHeaderData(3, Qt::Horizontal, "Midterm 2");
        gradesModel->setHeaderData(4, Qt::Horizontal, "Final");
        gradesModel->setHeaderData(5, Qt::Horizontal, "Practical");
        gradesModel->setHeaderData(6, Qt::Horizontal, "Assignments");
        gradesModel->setHeaderData(7, Qt::Horizontal, "Total");
        gradesModel->setHeaderData(8, Qt::Horizontal, "Grade");
        gradesModel->setHeaderData(9, Qt::Horizontal, "Published");
        // Column 10 holds the internal grade record id and is hidden from the view
        m_gradesTable->setColumnHidden(10, true);
    }
}

