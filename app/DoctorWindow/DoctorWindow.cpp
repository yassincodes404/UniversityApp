#include "DoctorWindow.h"
#include "../DAL/DoctorRepo.h"
#include "../DAL/CourseRepo.h"
#include "../DAL/DatabaseManager.h"
#include <QMenuBar>
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
    
    m_studentsTable = new QTableView(this);
    QSqlQueryModel* studentsModel = new QSqlQueryModel(m_studentsTable);
    m_studentsTable->setModel(studentsModel);
    m_studentsTable->horizontalHeader()->setStretchLastSection(true);
    
    studentsLayout->addWidget(refreshBtn);
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
    
    m_gradesTable = new QTableView(this);
    QSqlQueryModel* gradesModel = new QSqlQueryModel(m_gradesTable);
    m_gradesTable->setModel(gradesModel);
    m_gradesTable->horizontalHeader()->setStretchLastSection(true);
    
    gradesLayout->addLayout(gradesButtonLayout);
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
    QMessageBox::information(this, "Enter Grades", "Grade entry functionality - to be implemented");
}

void DoctorWindow::onMarkAttendance()
{
    QMessageBox::information(this, "Mark Attendance", "Attendance marking functionality - to be implemented");
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
    
    // Load students enrolled in this course
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(m_studentsTable->model());
    if (model) {
        QSqlQuery query(DatabaseManager::instance().database());
        query.prepare(
            "SELECT s.student_code, s.name, s.email, e.status "
            "FROM enrollments e "
            "JOIN students s ON e.student_id = s.id "
            "WHERE e.course_id = ? AND e.status = 'registered' "
            "ORDER BY s.student_code"
        );
        query.addBindValue(courseId);
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
        gradesQuery.prepare(
            "SELECT s.student_code, s.name, g.midterm1, g.midterm2, g.final_exam, "
            "g.practical, g.assignments, g.total, g.letter_grade, g.is_published "
            "FROM grades g "
            "JOIN students s ON g.student_id = s.id "
            "WHERE g.course_id = ? "
            "ORDER BY s.student_code"
        );
        gradesQuery.addBindValue(courseId);
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
    }
}

