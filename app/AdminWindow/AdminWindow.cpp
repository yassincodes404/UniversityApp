#include "AdminWindow.h"
#include "../DAL/StudentRepo.h"
#include "../DAL/CourseRepo.h"
#include "../DAL/DatabaseManager.h"
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QHBoxLayout>
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QSplitter>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QDialog>
#include <QFormLayout>
#include <QDateEdit>
#include <QComboBox>
#include <QSpinBox>

AdminWindow::AdminWindow(const User& admin, QWidget* parent)
    : QMainWindow(parent), m_admin(admin)
{
    setWindowTitle(QString("لوحة تحكم الإدارة - %1").arg(admin.name));
    setMinimumSize(1000, 700);
    
    setupUI();
    loadStudents();
    loadCourses();
}

AdminWindow::~AdminWindow()
{
}

void AdminWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    
    // Sidebar menu
    m_menuList = new QListWidget(this);
    m_menuList->addItem("لوحة التحكم");
    m_menuList->addItem("الطلاب");
    m_menuList->addItem("المقررات");
    m_menuList->addItem("التسجيلات");
    m_menuList->addItem("الدرجات");
    m_menuList->addItem("الحضور");
    m_menuList->addItem("الدفع");
    m_menuList->addItem("الأخبار");
    m_menuList->setMaximumWidth(200);
    m_menuList->setCurrentRow(0);
    
    connect(m_menuList, &QListWidget::currentRowChanged, this, &AdminWindow::onMenuSelectionChanged);
    
    // Content area
    m_stackedWidget = new QStackedWidget(this);
    
    setupDashboardPage();
    setupStudentsPage();
    setupCoursesPage();
    
    // Add placeholder pages for other sections
    for (int i = 3; i < 8; ++i) {
        QWidget* page = new QWidget();
        QLabel* label = new QLabel(QString("قسم %1 - قيد التطوير").arg(i + 1), page);
        label->setAlignment(Qt::AlignCenter);
        QVBoxLayout* layout = new QVBoxLayout(page);
        layout->addWidget(label);
        m_stackedWidget->addWidget(page);
    }
    
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(m_menuList);
    splitter->addWidget(m_stackedWidget);
    splitter->setStretchFactor(1, 1);
    
    mainLayout->addWidget(splitter);
}

void AdminWindow::setupDashboardPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    
    QLabel* title = new QLabel("لوحة التحكم", page);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(title);
    
    QLabel* welcome = new QLabel(QString("مرحباً %1").arg(m_admin.name), page);
    layout->addWidget(welcome);
    
    // Add summary cards here in future
    QLabel* summary = new QLabel("إحصائيات سريعة:\n- عدد الطلاب\n- عدد المقررات\n- التسجيلات المعلقة", page);
    layout->addWidget(summary);
    
    layout->addStretch();
    m_stackedWidget->addWidget(page);
}

void AdminWindow::setupStudentsPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    
    // Toolbar
    QHBoxLayout* toolbar = new QHBoxLayout();
    
    m_addStudentBtn = new QPushButton("إضافة طالب", page);
    m_editStudentBtn = new QPushButton("تعديل", page);
    m_deleteStudentBtn = new QPushButton("حذف", page);
    m_refreshStudentsBtn = new QPushButton("تحديث", page);
    
    m_searchStudentsEdit = new QLineEdit(page);
    m_searchStudentsEdit->setPlaceholderText("بحث...");
    
    toolbar->addWidget(m_addStudentBtn);
    toolbar->addWidget(m_editStudentBtn);
    toolbar->addWidget(m_deleteStudentBtn);
    toolbar->addWidget(m_refreshStudentsBtn);
    toolbar->addStretch();
    toolbar->addWidget(new QLabel("بحث:", page));
    toolbar->addWidget(m_searchStudentsEdit);
    
    connect(m_addStudentBtn, &QPushButton::clicked, this, &AdminWindow::onAddStudent);
    connect(m_editStudentBtn, &QPushButton::clicked, this, &AdminWindow::onEditStudent);
    connect(m_deleteStudentBtn, &QPushButton::clicked, this, &AdminWindow::onDeleteStudent);
    connect(m_refreshStudentsBtn, &QPushButton::clicked, this, &AdminWindow::onRefreshStudents);
    connect(m_searchStudentsEdit, &QLineEdit::textChanged, this, &AdminWindow::onSearchStudents);
    
    layout->addLayout(toolbar);
    
    // Table
    m_studentsTable = new QTableView(page);
    m_studentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_studentsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_studentsTable->horizontalHeader()->setStretchLastSection(true);
    
    layout->addWidget(m_studentsTable);
    
    m_stackedWidget->addWidget(page);
}

void AdminWindow::setupCoursesPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    
    // Toolbar
    QHBoxLayout* toolbar = new QHBoxLayout();
    
    m_addCourseBtn = new QPushButton("إضافة مقرر", page);
    m_editCourseBtn = new QPushButton("تعديل", page);
    m_deleteCourseBtn = new QPushButton("حذف", page);
    m_refreshCoursesBtn = new QPushButton("تحديث", page);
    
    m_searchCoursesEdit = new QLineEdit(page);
    m_searchCoursesEdit->setPlaceholderText("بحث...");
    
    toolbar->addWidget(m_addCourseBtn);
    toolbar->addWidget(m_editCourseBtn);
    toolbar->addWidget(m_deleteCourseBtn);
    toolbar->addWidget(m_refreshCoursesBtn);
    toolbar->addStretch();
    toolbar->addWidget(new QLabel("بحث:", page));
    toolbar->addWidget(m_searchCoursesEdit);
    
    connect(m_addCourseBtn, &QPushButton::clicked, this, &AdminWindow::onAddCourse);
    connect(m_editCourseBtn, &QPushButton::clicked, this, &AdminWindow::onEditCourse);
    connect(m_deleteCourseBtn, &QPushButton::clicked, this, &AdminWindow::onDeleteCourse);
    connect(m_refreshCoursesBtn, &QPushButton::clicked, this, &AdminWindow::onRefreshCourses);
    connect(m_searchCoursesEdit, &QLineEdit::textChanged, this, &AdminWindow::onSearchCourses);
    
    layout->addLayout(toolbar);
    
    // Table
    m_coursesTable = new QTableView(page);
    m_coursesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_coursesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_coursesTable->horizontalHeader()->setStretchLastSection(true);
    
    layout->addWidget(m_coursesTable);
    
    m_stackedWidget->addWidget(page);
}

void AdminWindow::loadStudents()
{
    StudentRepo repo;
    auto students = repo.findAll();
    
    QSqlQueryModel* model = new QSqlQueryModel(this);
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, student_code, name, email, phone, level, gpa, status FROM students ORDER BY student_code");
    query.exec();
    model->setQuery(query);
    
    model->setHeaderData(1, Qt::Horizontal, "الرقم الجامعي");
    model->setHeaderData(2, Qt::Horizontal, "الاسم");
    model->setHeaderData(3, Qt::Horizontal, "البريد");
    model->setHeaderData(4, Qt::Horizontal, "الهاتف");
    model->setHeaderData(5, Qt::Horizontal, "المستوى");
    model->setHeaderData(6, Qt::Horizontal, "المعدل");
    model->setHeaderData(7, Qt::Horizontal, "الحالة");
    
    m_studentsTable->setModel(model);
    m_studentsTable->hideColumn(0); // Hide ID column
}

void AdminWindow::loadCourses()
{
    QSqlQueryModel* model = new QSqlQueryModel(this);
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, code, name, credit_hours, year, semester, course_type FROM courses ORDER BY code");
    query.exec();
    model->setQuery(query);
    
    model->setHeaderData(1, Qt::Horizontal, "الكود");
    model->setHeaderData(2, Qt::Horizontal, "الاسم");
    model->setHeaderData(3, Qt::Horizontal, "الساعات");
    model->setHeaderData(4, Qt::Horizontal, "السنة");
    model->setHeaderData(5, Qt::Horizontal, "الفصل");
    model->setHeaderData(6, Qt::Horizontal, "النوع");
    
    m_coursesTable->setModel(model);
    m_coursesTable->hideColumn(0); // Hide ID column
}

void AdminWindow::onMenuSelectionChanged()
{
    int row = m_menuList->currentRow();
    if (row >= 0 && row < m_stackedWidget->count()) {
        m_stackedWidget->setCurrentIndex(row);
    }
}

void AdminWindow::onAddStudent()
{
    QMessageBox::information(this, "إضافة طالب", "نموذج إضافة طالب - قيد التطوير");
    // TODO: Implement student form dialog
}

void AdminWindow::onEditStudent()
{
    QModelIndex index = m_studentsTable->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "تحذير", "يرجى اختيار طالب للتعديل");
        return;
    }
    
    QMessageBox::information(this, "تعديل طالب", "نموذج تعديل طالب - قيد التطوير");
    // TODO: Implement student edit dialog
}

void AdminWindow::onDeleteStudent()
{
    QModelIndex index = m_studentsTable->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "تحذير", "يرجى اختيار طالب للحذف");
        return;
    }
    
    int ret = QMessageBox::question(this, "تأكيد", "هل أنت متأكد من حذف هذا الطالب؟",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        // TODO: Implement delete
        QMessageBox::information(this, "حذف", "تم الحذف بنجاح");
        loadStudents();
    }
}

void AdminWindow::onAddCourse()
{
    QMessageBox::information(this, "إضافة مقرر", "نموذج إضافة مقرر - قيد التطوير");
    // TODO: Implement course form dialog
}

void AdminWindow::onEditCourse()
{
    QModelIndex index = m_coursesTable->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "تحذير", "يرجى اختيار مقرر للتعديل");
        return;
    }
    
    QMessageBox::information(this, "تعديل مقرر", "نموذج تعديل مقرر - قيد التطوير");
    // TODO: Implement course edit dialog
}

void AdminWindow::onDeleteCourse()
{
    QModelIndex index = m_coursesTable->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "تحذير", "يرجى اختيار مقرر للحذف");
        return;
    }
    
    int ret = QMessageBox::question(this, "تأكيد", "هل أنت متأكد من حذف هذا المقرر؟",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        // TODO: Implement delete
        QMessageBox::information(this, "حذف", "تم الحذف بنجاح");
        loadCourses();
    }
}

void AdminWindow::onRefreshStudents()
{
    loadStudents();
}

void AdminWindow::onRefreshCourses()
{
    loadCourses();
}

void AdminWindow::onSearchStudents()
{
    QString search = m_searchStudentsEdit->text();
    // TODO: Implement search filtering
}

void AdminWindow::onSearchCourses()
{
    QString search = m_searchCoursesEdit->text();
    // TODO: Implement search filtering
}

