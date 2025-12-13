#include "DoctorWindow.h"
#include "../DAL/DatabaseManager.h"
#include "../Services/GradingService.h"
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QHBoxLayout>
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QSplitter>
#include <QHeaderView>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>

DoctorWindow::DoctorWindow(const User& doctor, QWidget* parent)
    : QMainWindow(parent), m_doctor(doctor)
{
    setWindowTitle(QString("لوحة تحكم الدكتور - %1").arg(doctor.name));
    setMinimumSize(1000, 700);
    
    setupUI();
    loadCourses();
}

DoctorWindow::~DoctorWindow()
{
}

void DoctorWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    
    // Sidebar menu
    m_menuList = new QListWidget(this);
    m_menuList->addItem("لوحة التحكم");
    m_menuList->addItem("مقرراتي");
    m_menuList->addItem("إدخال الدرجات");
    m_menuList->addItem("تسجيل الحضور");
    m_menuList->setMaximumWidth(200);
    m_menuList->setCurrentRow(0);
    
    connect(m_menuList, &QListWidget::currentRowChanged, this, &DoctorWindow::onMenuSelectionChanged);
    
    // Content area
    m_stackedWidget = new QStackedWidget(this);
    
    setupDashboardPage();
    setupCoursesPage();
    setupGradeEntryPage();
    setupAttendancePage();
    
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(m_menuList);
    splitter->addWidget(m_stackedWidget);
    splitter->setStretchFactor(1, 1);
    
    mainLayout->addWidget(splitter);
}

void DoctorWindow::setupDashboardPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    
    QLabel* title = new QLabel("لوحة التحكم", page);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(title);
    
    QLabel* welcome = new QLabel(QString("مرحباً د. %1").arg(m_doctor.name), page);
    layout->addWidget(welcome);
    
    QLabel* summary = new QLabel("المقررات المخصصة:\n- عدد المقررات\n- عدد الطلاب", page);
    layout->addWidget(summary);
    
    layout->addStretch();
    m_stackedWidget->addWidget(page);
}

void DoctorWindow::setupCoursesPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    
    QLabel* title = new QLabel("مقرراتي", page);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(title);
    
    m_coursesTable = new QTableView(page);
    m_coursesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_coursesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_coursesTable->horizontalHeader()->setStretchLastSection(true);
    
    connect(m_coursesTable, &QTableView::doubleClicked, this, &DoctorWindow::onCourseSelected);
    
    layout->addWidget(m_coursesTable);
    
    m_stackedWidget->addWidget(page);
}

void DoctorWindow::setupGradeEntryPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    
    QLabel* title = new QLabel("إدخال الدرجات", page);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(title);
    
    QHBoxLayout* toolbar = new QHBoxLayout();
    toolbar->addWidget(new QLabel("اختر المقرر:", page));
    
    m_courseCombo = new QComboBox(page);
    connect(m_courseCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &DoctorWindow::onCourseSelected);
    toolbar->addWidget(m_courseCombo);
    toolbar->addStretch();
    
    m_saveGradesBtn = new QPushButton("حفظ الدرجات", page);
    connect(m_saveGradesBtn, &QPushButton::clicked, this, &DoctorWindow::onEnterGrades);
    toolbar->addWidget(m_saveGradesBtn);
    
    layout->addLayout(toolbar);
    
    m_studentsTable = new QTableView(page);
    m_studentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_studentsTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(m_studentsTable);
    
    m_stackedWidget->addWidget(page);
}

void DoctorWindow::setupAttendancePage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    
    QLabel* title = new QLabel("تسجيل الحضور", page);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(title);
    
    m_markAttendanceBtn = new QPushButton("تسجيل الحضور", page);
    connect(m_markAttendanceBtn, &QPushButton::clicked, this, &DoctorWindow::onMarkAttendance);
    layout->addWidget(m_markAttendanceBtn);
    
    m_attendanceTable = new QTableView(page);
    m_attendanceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_attendanceTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(m_attendanceTable);
    
    m_stackedWidget->addWidget(page);
}

void DoctorWindow::loadCourses()
{
    QSqlQueryModel* model = new QSqlQueryModel(this);
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT DISTINCT c.id, c.code, c.name, c.credit_hours "
                  "FROM courses c "
                  "JOIN sections s ON c.id = s.course_id "
                  "WHERE s.doctor_id = :doctor_id "
                  "ORDER BY c.code");
    query.bindValue(":doctor_id", m_doctor.id);
    query.exec();
    model->setQuery(query);
    
    model->setHeaderData(1, Qt::Horizontal, "الكود");
    model->setHeaderData(2, Qt::Horizontal, "الاسم");
    model->setHeaderData(3, Qt::Horizontal, "الساعات");
    
    m_coursesTable->setModel(model);
    m_coursesTable->hideColumn(0);
    
    // Populate combo box
    m_courseCombo->clear();
    query.exec();
    while (query.next()) {
        int id = query.value(0).toInt();
        QString code = query.value(1).toString();
        QString name = query.value(2).toString();
        m_courseCombo->addItem(QString("%1 - %2").arg(code, name), id);
    }
}

void DoctorWindow::loadStudentsForCourse(int courseId)
{
    QSqlQueryModel* model = new QSqlQueryModel(this);
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT s.id, s.student_code, s.name, "
                  "COALESCE(g.midterm1, 0) as m1, "
                  "COALESCE(g.midterm2, 0) as m2, "
                  "COALESCE(g.final_exam, 0) as final, "
                  "COALESCE(g.practical, 0) as prac, "
                  "COALESCE(g.assignments, 0) as assign, "
                  "COALESCE(g.total, 0) as total "
                  "FROM enrollments e "
                  "JOIN students s ON e.student_id = s.id "
                  "LEFT JOIN grades g ON s.id = g.student_id AND e.course_id = g.course_id "
                  "WHERE e.course_id = :course_id AND e.status = 'registered' "
                  "ORDER BY s.student_code");
    query.bindValue(":course_id", courseId);
    query.exec();
    model->setQuery(query);
    
    model->setHeaderData(1, Qt::Horizontal, "الرقم الجامعي");
    model->setHeaderData(2, Qt::Horizontal, "الاسم");
    model->setHeaderData(3, Qt::Horizontal, "نصفي 1");
    model->setHeaderData(4, Qt::Horizontal, "نصفي 2");
    model->setHeaderData(5, Qt::Horizontal, "نهائي");
    model->setHeaderData(6, Qt::Horizontal, "عملي");
    model->setHeaderData(7, Qt::Horizontal, "مشاريع");
    model->setHeaderData(8, Qt::Horizontal, "المجموع");
    
    m_studentsTable->setModel(model);
    m_studentsTable->hideColumn(0);
}

void DoctorWindow::onMenuSelectionChanged()
{
    int row = m_menuList->currentRow();
    if (row >= 0 && row < m_stackedWidget->count()) {
        m_stackedWidget->setCurrentIndex(row);
    }
}

void DoctorWindow::onCourseSelected()
{
    int courseId = -1;
    
    if (m_courseCombo->currentIndex() >= 0) {
        courseId = m_courseCombo->currentData().toInt();
    } else if (m_coursesTable->currentIndex().isValid()) {
        QModelIndex index = m_coursesTable->currentIndex();
        QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(m_coursesTable->model());
        if (model) {
            QModelIndex idIndex = model->index(index.row(), 0);
            courseId = model->data(idIndex).toInt();
        }
    }
    
    if (courseId > 0) {
        loadStudentsForCourse(courseId);
    }
}

void DoctorWindow::onEnterGrades()
{
    QMessageBox::information(this, "حفظ الدرجات", "تم حفظ الدرجات بنجاح");
    // TODO: Implement grade saving logic
}

void DoctorWindow::onMarkAttendance()
{
    QMessageBox::information(this, "تسجيل الحضور", "نموذج تسجيل الحضور - قيد التطوير");
    // TODO: Implement attendance marking
}

