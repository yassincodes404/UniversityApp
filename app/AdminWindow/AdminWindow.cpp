#include "AdminWindow.h"
#include "../DAL/StudentRepo.h"
#include "../DAL/CourseRepo.h"
#include "../DAL/DatabaseManager.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QTabWidget>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QHeaderView>
#include <QSqlDatabase>

AdminWindow::AdminWindow(int adminId, QWidget *parent)
    : QMainWindow(parent)
    , m_adminId(adminId)
{
    setupUI();
    setupMenuBar();
    setWindowTitle("Admin Portal - NCTU SIS");
    resize(1200, 800);
}

AdminWindow::~AdminWindow()
{
}

void AdminWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Logout button
    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addStretch();
    m_logoutButton = new QPushButton("Logout", this);
    connect(m_logoutButton, &QPushButton::clicked, this, &AdminWindow::onLogout);
    topLayout->addWidget(m_logoutButton);
    mainLayout->addLayout(topLayout);
    
    // Tabs
    m_tabs = new QTabWidget(this);
    
    // Students tab
    QWidget* studentsWidget = new QWidget;
    QVBoxLayout* studentsLayout = new QVBoxLayout(studentsWidget);
    
    QHBoxLayout* studentsButtonLayout = new QHBoxLayout;
    QPushButton* addStudentBtn = new QPushButton("Add Student", this);
    QPushButton* editStudentBtn = new QPushButton("Edit Student", this);
    QPushButton* deleteStudentBtn = new QPushButton("Delete Student", this);
    QPushButton* refreshStudentsBtn = new QPushButton("Refresh", this);
    
    connect(addStudentBtn, &QPushButton::clicked, this, &AdminWindow::onAddStudent);
    connect(editStudentBtn, &QPushButton::clicked, this, &AdminWindow::onEditStudent);
    connect(deleteStudentBtn, &QPushButton::clicked, this, &AdminWindow::onDeleteStudent);
    connect(refreshStudentsBtn, &QPushButton::clicked, this, &AdminWindow::refreshStudents);
    
    studentsButtonLayout->addWidget(addStudentBtn);
    studentsButtonLayout->addWidget(editStudentBtn);
    studentsButtonLayout->addWidget(deleteStudentBtn);
    studentsButtonLayout->addStretch();
    studentsButtonLayout->addWidget(refreshStudentsBtn);
    
    m_studentsTable = new QTableView(this);
    QSqlTableModel* studentsModel = new QSqlTableModel(m_studentsTable, DatabaseManager::instance().database());
    studentsModel->setTable("students");
    studentsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    studentsModel->select();
    studentsModel->setHeaderData(1, Qt::Horizontal, "Student Code");
    studentsModel->setHeaderData(2, Qt::Horizontal, "Name");
    studentsModel->setHeaderData(3, Qt::Horizontal, "Email");
    studentsModel->setHeaderData(4, Qt::Horizontal, "Phone");
    studentsModel->setHeaderData(12, Qt::Horizontal, "Status");
    studentsModel->setHeaderData(13, Qt::Horizontal, "Level");
    
    m_studentsTable->setModel(studentsModel);
    m_studentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_studentsTable->horizontalHeader()->setStretchLastSection(true);
    m_studentsTable->resizeColumnsToContents();
    
    studentsLayout->addLayout(studentsButtonLayout);
    studentsLayout->addWidget(m_studentsTable);
    
    m_tabs->addTab(studentsWidget, "Students");
    
    // Courses tab
    QWidget* coursesWidget = new QWidget;
    QVBoxLayout* coursesLayout = new QVBoxLayout(coursesWidget);
    
    QHBoxLayout* coursesButtonLayout = new QHBoxLayout;
    QPushButton* addCourseBtn = new QPushButton("Add Course", this);
    QPushButton* editCourseBtn = new QPushButton("Edit Course", this);
    QPushButton* deleteCourseBtn = new QPushButton("Delete Course", this);
    QPushButton* refreshCoursesBtn = new QPushButton("Refresh", this);
    
    connect(addCourseBtn, &QPushButton::clicked, this, &AdminWindow::onAddCourse);
    connect(editCourseBtn, &QPushButton::clicked, this, &AdminWindow::onEditCourse);
    connect(deleteCourseBtn, &QPushButton::clicked, this, &AdminWindow::onDeleteCourse);
    connect(refreshCoursesBtn, &QPushButton::clicked, this, &AdminWindow::refreshCourses);
    
    coursesButtonLayout->addWidget(addCourseBtn);
    coursesButtonLayout->addWidget(editCourseBtn);
    coursesButtonLayout->addWidget(deleteCourseBtn);
    coursesButtonLayout->addStretch();
    coursesButtonLayout->addWidget(refreshCoursesBtn);
    
    m_coursesTable = new QTableView(this);
    QSqlTableModel* coursesModel = new QSqlTableModel(m_coursesTable, DatabaseManager::instance().database());
    coursesModel->setTable("courses");
    coursesModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    coursesModel->select();
    coursesModel->setHeaderData(1, Qt::Horizontal, "Code");
    coursesModel->setHeaderData(2, Qt::Horizontal, "Name");
    coursesModel->setHeaderData(3, Qt::Horizontal, "Credit Hours");
    coursesModel->setHeaderData(5, Qt::Horizontal, "Year");
    coursesModel->setHeaderData(6, Qt::Horizontal, "Semester");
    
    m_coursesTable->setModel(coursesModel);
    m_coursesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_coursesTable->horizontalHeader()->setStretchLastSection(true);
    m_coursesTable->resizeColumnsToContents();
    
    coursesLayout->addLayout(coursesButtonLayout);
    coursesLayout->addWidget(m_coursesTable);
    
    m_tabs->addTab(coursesWidget, "Courses");
    
    mainLayout->addWidget(m_tabs);
    
    statusBar()->showMessage("Welcome, Admin");
}

void AdminWindow::setupMenuBar()
{
    QMenu* fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("Logout", this, &AdminWindow::onLogout);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close);
}

void AdminWindow::onLogout()
{
    emit logoutRequested();
}

void AdminWindow::onAddStudent()
{
    QMessageBox::information(this, "Add Student", "Add student functionality - to be implemented");
}

void AdminWindow::onEditStudent()
{
    QMessageBox::information(this, "Edit Student", "Edit student functionality - to be implemented");
}

void AdminWindow::onDeleteStudent()
{
    QMessageBox::information(this, "Delete Student", "Delete student functionality - to be implemented");
}

void AdminWindow::onAddCourse()
{
    QMessageBox::information(this, "Add Course", "Add course functionality - to be implemented");
}

void AdminWindow::onEditCourse()
{
    QMessageBox::information(this, "Edit Course", "Edit course functionality - to be implemented");
}

void AdminWindow::onDeleteCourse()
{
    QMessageBox::information(this, "Delete Course", "Delete course functionality - to be implemented");
}

void AdminWindow::refreshStudents()
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(m_studentsTable->model());
    if (model) {
        model->select();
    }
}

void AdminWindow::refreshCourses()
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(m_coursesTable->model());
    if (model) {
        model->select();
    }
}

