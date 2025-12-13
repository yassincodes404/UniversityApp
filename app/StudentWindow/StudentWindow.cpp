#include "StudentWindow.h"
#include "../DAL/StudentRepo.h"
#include "../DAL/EnrollmentRepo.h"
#include "../DAL/GradeRepo.h"
#include "../DAL/PaymentRepo.h"
#include "../DAL/NewsRepo.h"
#include "../DAL/DatabaseManager.h"
#include <QMenuBar>
#include <QMenu>
#include <QSqlQueryModel>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QTabWidget>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QHeaderView>
#include <QLabel>

StudentWindow::StudentWindow(int studentId, QWidget *parent)
    : QMainWindow(parent)
    , m_studentId(studentId)
{
    setupUI();
    setupMenuBar();
    loadProfile();
    setWindowTitle("Student Portal - NCTU SIS");
    resize(1000, 700);
}

StudentWindow::~StudentWindow()
{
}

void StudentWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Logout button
    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addStretch();
    m_logoutButton = new QPushButton("Logout", this);
    connect(m_logoutButton, &QPushButton::clicked, this, &StudentWindow::onLogout);
    topLayout->addWidget(m_logoutButton);
    mainLayout->addLayout(topLayout);
    
    // Tabs
    m_tabs = new QTabWidget(this);
    
    // Profile tab
    QWidget* profileWidget = new QWidget;
    QVBoxLayout* profileLayout = new QVBoxLayout(profileWidget);
    m_profileLabel = new QLabel(this);
    m_profileLabel->setTextFormat(Qt::RichText);
    profileLayout->addWidget(m_profileLabel);
    m_tabs->addTab(profileWidget, "Profile");
    
    // Courses tab
    QWidget* coursesWidget = new QWidget;
    QVBoxLayout* coursesLayout = new QVBoxLayout(coursesWidget);
    
    QPushButton* refreshBtn = new QPushButton("Refresh", this);
    connect(refreshBtn, &QPushButton::clicked, this, &StudentWindow::refreshData);
    
    m_coursesTable = new QTableView(this);
    QSqlQueryModel* coursesModel = new QSqlQueryModel(m_coursesTable);
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT c.code, c.name, c.credit_hours, e.semester, e.year, e.status "
        "FROM enrollments e "
        "JOIN courses c ON e.course_id = c.id "
        "WHERE e.student_id = ? AND e.status = 'registered' "
        "ORDER BY e.year DESC, e.semester DESC"
    );
    query.addBindValue(m_studentId);
    query.exec();
    coursesModel->setQuery(query);
    coursesModel->setHeaderData(0, Qt::Horizontal, "Code");
    coursesModel->setHeaderData(1, Qt::Horizontal, "Name");
    coursesModel->setHeaderData(2, Qt::Horizontal, "Credits");
    coursesModel->setHeaderData(3, Qt::Horizontal, "Semester");
    coursesModel->setHeaderData(4, Qt::Horizontal, "Year");
    coursesModel->setHeaderData(5, Qt::Horizontal, "Status");
    
    m_coursesTable->setModel(coursesModel);
    m_coursesTable->horizontalHeader()->setStretchLastSection(true);
    
    coursesLayout->addWidget(refreshBtn);
    coursesLayout->addWidget(m_coursesTable);
    m_tabs->addTab(coursesWidget, "My Courses");
    
    // Grades tab
    QWidget* gradesWidget = new QWidget;
    QVBoxLayout* gradesLayout = new QVBoxLayout(gradesWidget);
    
    m_gradesTable = new QTableView(this);
    QSqlQueryModel* gradesModel = new QSqlQueryModel(m_gradesTable);
    QSqlQuery gradesQuery(DatabaseManager::instance().database());
    gradesQuery.prepare(
        "SELECT c.code, c.name, g.midterm1, g.midterm2, g.final_exam, "
        "g.practical, g.assignments, g.total, g.letter_grade, g.result_status "
        "FROM grades g "
        "JOIN courses c ON g.course_id = c.id "
        "WHERE g.student_id = ? AND g.is_published = 1 "
        "ORDER BY g.recorded_at DESC"
    );
    gradesQuery.addBindValue(m_studentId);
    gradesQuery.exec();
    gradesModel->setQuery(gradesQuery);
    gradesModel->setHeaderData(0, Qt::Horizontal, "Code");
    gradesModel->setHeaderData(1, Qt::Horizontal, "Course");
    gradesModel->setHeaderData(2, Qt::Horizontal, "Midterm 1");
    gradesModel->setHeaderData(3, Qt::Horizontal, "Midterm 2");
    gradesModel->setHeaderData(4, Qt::Horizontal, "Final");
    gradesModel->setHeaderData(5, Qt::Horizontal, "Practical");
    gradesModel->setHeaderData(6, Qt::Horizontal, "Assignments");
    gradesModel->setHeaderData(7, Qt::Horizontal, "Total");
    gradesModel->setHeaderData(8, Qt::Horizontal, "Grade");
    gradesModel->setHeaderData(9, Qt::Horizontal, "Status");
    
    m_gradesTable->setModel(gradesModel);
    m_gradesTable->horizontalHeader()->setStretchLastSection(true);
    
    gradesLayout->addWidget(m_gradesTable);
    m_tabs->addTab(gradesWidget, "Grades");
    
    // Payments tab
    QWidget* paymentsWidget = new QWidget;
    QVBoxLayout* paymentsLayout = new QVBoxLayout(paymentsWidget);
    
    m_paymentsTable = new QTableView(this);
    QSqlQueryModel* paymentsModel = new QSqlQueryModel(m_paymentsTable);
    QSqlQuery paymentsQuery(DatabaseManager::instance().database());
    paymentsQuery.prepare(
        "SELECT amount, semester, payment_date, method, status, notes "
        "FROM payments WHERE student_id = ? ORDER BY payment_date DESC"
    );
    paymentsQuery.addBindValue(m_studentId);
    paymentsQuery.exec();
    paymentsModel->setQuery(paymentsQuery);
    paymentsModel->setHeaderData(0, Qt::Horizontal, "Amount");
    paymentsModel->setHeaderData(1, Qt::Horizontal, "Semester");
    paymentsModel->setHeaderData(2, Qt::Horizontal, "Date");
    paymentsModel->setHeaderData(3, Qt::Horizontal, "Method");
    paymentsModel->setHeaderData(4, Qt::Horizontal, "Status");
    paymentsModel->setHeaderData(5, Qt::Horizontal, "Notes");
    
    m_paymentsTable->setModel(paymentsModel);
    m_paymentsTable->horizontalHeader()->setStretchLastSection(true);
    
    paymentsLayout->addWidget(m_paymentsTable);
    m_tabs->addTab(paymentsWidget, "Payments");
    
    // News tab
    QWidget* newsWidget = new QWidget;
    QVBoxLayout* newsLayout = new QVBoxLayout(newsWidget);
    
    m_newsTable = new QTableView(this);
    QSqlTableModel* newsModel = new QSqlTableModel(m_newsTable, DatabaseManager::instance().database());
    newsModel->setTable("news");
    newsModel->setFilter("1=1"); // Show all
    newsModel->setSort(2, Qt::DescendingOrder); // Sort by news_date
    newsModel->select();
    newsModel->setHeaderData(1, Qt::Horizontal, "Title");
    newsModel->setHeaderData(2, Qt::Horizontal, "Content");
    newsModel->setHeaderData(3, Qt::Horizontal, "Date");
    
    m_newsTable->setModel(newsModel);
    m_newsTable->setColumnHidden(0, true); // Hide ID
    m_newsTable->setColumnHidden(4, true); // Hide created_by
    m_newsTable->setColumnHidden(5, true); // Hide created_at
    m_newsTable->horizontalHeader()->setStretchLastSection(true);
    
    newsLayout->addWidget(m_newsTable);
    m_tabs->addTab(newsWidget, "News & Announcements");
    
    mainLayout->addWidget(m_tabs);
    
    statusBar()->showMessage("Welcome, Student");
}

void StudentWindow::setupMenuBar()
{
    QMenu* fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("Refresh", this, &StudentWindow::refreshData);
    fileMenu->addSeparator();
    fileMenu->addAction("Logout", this, &StudentWindow::onLogout);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close);
}

void StudentWindow::onLogout()
{
    emit logoutRequested();
}

void StudentWindow::refreshData()
{
    loadProfile();
    loadCourses();
    loadGrades();
    loadPayments();
    loadNews();
}

void StudentWindow::loadProfile()
{
    StudentRepo repo;
    auto student = repo.findById(m_studentId);
    if (student.has_value()) {
        QString profileText = QString(
            "<h2>Student Profile</h2>"
            "<p><b>Name:</b> %1</p>"
            "<p><b>Student Code:</b> %2</p>"
            "<p><b>Email:</b> %3</p>"
            "<p><b>Phone:</b> %4</p>"
            "<p><b>Level:</b> %5</p>"
            "<p><b>Status:</b> %6</p>"
            "<p><b>GPA:</b> %7</p>"
            "<p><b>Credits Completed:</b> %8</p>"
        ).arg(student->name())
         .arg(student->studentCode())
         .arg(student->email())
         .arg(student->phone())
         .arg(student->level())
         .arg(student->status())
         .arg(QString::number(student->gpa(), 'f', 2))
         .arg(student->creditsCompleted());
        
        m_profileLabel->setText(profileText);
    }
}

void StudentWindow::loadCourses()
{
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(m_coursesTable->model());
    if (model) {
        QSqlQuery query(DatabaseManager::instance().database());
        query.prepare(
            "SELECT c.code, c.name, c.credit_hours, e.semester, e.year, e.status "
            "FROM enrollments e "
            "JOIN courses c ON e.course_id = c.id "
            "WHERE e.student_id = ? AND e.status = 'registered' "
            "ORDER BY e.year DESC, e.semester DESC"
        );
        query.addBindValue(m_studentId);
        query.exec();
        model->setQuery(query);
    }
}

void StudentWindow::loadGrades()
{
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(m_gradesTable->model());
    if (model) {
        QSqlQuery query(DatabaseManager::instance().database());
        query.prepare(
            "SELECT c.code, c.name, g.midterm1, g.midterm2, g.final_exam, "
            "g.practical, g.assignments, g.total, g.letter_grade, g.result_status "
            "FROM grades g "
            "JOIN courses c ON g.course_id = c.id "
            "WHERE g.student_id = ? AND g.is_published = 1 "
            "ORDER BY g.recorded_at DESC"
        );
        query.addBindValue(m_studentId);
        query.exec();
        model->setQuery(query);
    }
}

void StudentWindow::loadPayments()
{
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(m_paymentsTable->model());
    if (model) {
        QSqlQuery query(DatabaseManager::instance().database());
        query.prepare(
            "SELECT amount, semester, payment_date, method, status, notes "
            "FROM payments WHERE student_id = ? ORDER BY payment_date DESC"
        );
        query.addBindValue(m_studentId);
        query.exec();
        model->setQuery(query);
    }
}

void StudentWindow::loadNews()
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(m_newsTable->model());
    if (model) {
        model->select();
    }
}

