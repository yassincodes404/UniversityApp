#include "StudentWindow.h"
#include "../DAL/DatabaseManager.h"
#include "../DAL/EnrollmentRepo.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QHBoxLayout>
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>
#include <QHeaderView>
#include <QSqlQueryModel>
#include <QSqlQuery>

StudentWindow::StudentWindow(const Student& student, QWidget* parent)
    : QMainWindow(parent), m_student(student)
{
    setWindowTitle(QString("بوابة الطالب - %1").arg(student.name));
    setMinimumSize(900, 600);
    
    setupUI();
    loadProfile();
    loadCourses();
    loadGrades();
    loadPayments();
    loadNews();
}

StudentWindow::~StudentWindow()
{
}

void StudentWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    
    // Sidebar menu
    m_menuList = new QListWidget(this);
    m_menuList->addItem("الملف الشخصي");
    m_menuList->addItem("مقرراتي");
    m_menuList->addItem("الدرجات");
    m_menuList->addItem("الدفع");
    m_menuList->addItem("الأخبار");
    m_menuList->addItem("تسجيل المقررات");
    m_menuList->setMaximumWidth(200);
    m_menuList->setCurrentRow(0);
    
    connect(m_menuList, &QListWidget::currentRowChanged, this, &StudentWindow::onMenuSelectionChanged);
    
    // Content area
    m_stackedWidget = new QStackedWidget(this);
    
    setupProfilePage();
    setupCoursesPage();
    setupGradesPage();
    setupPaymentsPage();
    setupNewsPage();
    
    // Add registration page placeholder
    QWidget* regPage = new QWidget();
    QLabel* regLabel = new QLabel("تسجيل المقررات - قيد التطوير", regPage);
    regLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout* regLayout = new QVBoxLayout(regPage);
    regLayout->addWidget(regLabel);
    m_stackedWidget->addWidget(regPage);
    
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(m_menuList);
    splitter->addWidget(m_stackedWidget);
    splitter->setStretchFactor(1, 1);
    
    mainLayout->addWidget(splitter);
}

void StudentWindow::setupProfilePage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    
    QLabel* title = new QLabel("الملف الشخصي", page);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(title);
    
    QFormLayout* formLayout = new QFormLayout();
    
    m_nameLabel = new QLabel(page);
    m_codeLabel = new QLabel(page);
    m_emailLabel = new QLabel(page);
    m_phoneLabel = new QLabel(page);
    m_levelLabel = new QLabel(page);
    m_gpaLabel = new QLabel(page);
    m_creditsLabel = new QLabel(page);
    
    formLayout->addRow("الاسم:", m_nameLabel);
    formLayout->addRow("الرقم الجامعي:", m_codeLabel);
    formLayout->addRow("البريد الإلكتروني:", m_emailLabel);
    formLayout->addRow("الهاتف:", m_phoneLabel);
    formLayout->addRow("المستوى:", m_levelLabel);
    formLayout->addRow("المعدل التراكمي:", m_gpaLabel);
    formLayout->addRow("الساعات المكتملة:", m_creditsLabel);
    
    layout->addLayout(formLayout);
    layout->addStretch();
    
    m_stackedWidget->addWidget(page);
}

void StudentWindow::setupCoursesPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    
    QLabel* title = new QLabel("مقرراتي", page);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(title);
    
    QPushButton* refreshBtn = new QPushButton("تحديث", page);
    connect(refreshBtn, &QPushButton::clicked, this, &StudentWindow::onRefreshData);
    layout->addWidget(refreshBtn);
    
    m_coursesTable = new QTableView(page);
    m_coursesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_coursesTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(m_coursesTable);
    
    m_stackedWidget->addWidget(page);
}

void StudentWindow::setupGradesPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    
    QLabel* title = new QLabel("الدرجات", page);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(title);
    
    m_gradesTable = new QTableView(page);
    m_gradesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_gradesTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(m_gradesTable);
    
    m_stackedWidget->addWidget(page);
}

void StudentWindow::setupPaymentsPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    
    QLabel* title = new QLabel("الدفع والرسوم", page);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(title);
    
    m_paymentsTable = new QTableView(page);
    m_paymentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_paymentsTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(m_paymentsTable);
    
    m_stackedWidget->addWidget(page);
}

void StudentWindow::setupNewsPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    
    QLabel* title = new QLabel("الأخبار والإعلانات", page);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(title);
    
    QLabel* newsLabel = new QLabel("آخر الأخبار:", page);
    layout->addWidget(newsLabel);
    
    // TODO: Add news list widget
    QLabel* placeholder = new QLabel("لا توجد أخبار حالياً", page);
    placeholder->setAlignment(Qt::AlignCenter);
    layout->addWidget(placeholder);
    
    layout->addStretch();
    m_stackedWidget->addWidget(page);
}

void StudentWindow::loadProfile()
{
    m_nameLabel->setText(m_student.name);
    m_codeLabel->setText(m_student.studentCode);
    m_emailLabel->setText(m_student.email);
    m_phoneLabel->setText(m_student.phone);
    m_levelLabel->setText(QString::number(m_student.level));
    m_gpaLabel->setText(QString::number(m_student.gpa, 'f', 2));
    m_creditsLabel->setText(QString::number(m_student.creditsCompleted));
}

void StudentWindow::loadCourses()
{
    EnrollmentRepo repo;
    auto enrollments = repo.findByStudent(m_student.id);
    
    QSqlQueryModel* model = new QSqlQueryModel(this);
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT c.code, c.name, c.credit_hours, e.semester, e.year, e.status "
                  "FROM enrollments e "
                  "JOIN courses c ON e.course_id = c.id "
                  "WHERE e.student_id = :student_id AND e.status = 'registered' "
                  "ORDER BY e.year DESC, e.semester DESC");
    query.bindValue(":student_id", m_student.id);
    query.exec();
    model->setQuery(query);
    
    model->setHeaderData(0, Qt::Horizontal, "كود المقرر");
    model->setHeaderData(1, Qt::Horizontal, "اسم المقرر");
    model->setHeaderData(2, Qt::Horizontal, "الساعات");
    model->setHeaderData(3, Qt::Horizontal, "الفصل");
    model->setHeaderData(4, Qt::Horizontal, "السنة");
    model->setHeaderData(5, Qt::Horizontal, "الحالة");
    
    m_coursesTable->setModel(model);
}

void StudentWindow::loadGrades()
{
    QSqlQueryModel* model = new QSqlQueryModel(this);
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT c.code, c.name, g.midterm1, g.midterm2, g.final_exam, "
                  "g.practical, g.assignments, g.total, g.letter_grade, g.result_status "
                  "FROM grades g "
                  "JOIN courses c ON g.course_id = c.id "
                  "WHERE g.student_id = :student_id "
                  "ORDER BY c.code");
    query.bindValue(":student_id", m_student.id);
    query.exec();
    model->setQuery(query);
    
    model->setHeaderData(0, Qt::Horizontal, "الكود");
    model->setHeaderData(1, Qt::Horizontal, "المقرر");
    model->setHeaderData(2, Qt::Horizontal, "نصفي 1");
    model->setHeaderData(3, Qt::Horizontal, "نصفي 2");
    model->setHeaderData(4, Qt::Horizontal, "نهائي");
    model->setHeaderData(5, Qt::Horizontal, "عملي");
    model->setHeaderData(6, Qt::Horizontal, "مشاريع");
    model->setHeaderData(7, Qt::Horizontal, "المجموع");
    model->setHeaderData(8, Qt::Horizontal, "الدرجة");
    model->setHeaderData(9, Qt::Horizontal, "الحالة");
    
    m_gradesTable->setModel(model);
}

void StudentWindow::loadPayments()
{
    QSqlQueryModel* model = new QSqlQueryModel(this);
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT amount, semester, payment_date, method, status, notes "
                  "FROM payments WHERE student_id = :student_id ORDER BY payment_date DESC");
    query.bindValue(":student_id", m_student.id);
    query.exec();
    model->setQuery(query);
    
    model->setHeaderData(0, Qt::Horizontal, "المبلغ");
    model->setHeaderData(1, Qt::Horizontal, "الفصل");
    model->setHeaderData(2, Qt::Horizontal, "تاريخ الدفع");
    model->setHeaderData(3, Qt::Horizontal, "الطريقة");
    model->setHeaderData(4, Qt::Horizontal, "الحالة");
    model->setHeaderData(5, Qt::Horizontal, "ملاحظات");
    
    m_paymentsTable->setModel(model);
}

void StudentWindow::loadNews()
{
    // TODO: Implement news loading
}

void StudentWindow::onMenuSelectionChanged()
{
    int row = m_menuList->currentRow();
    if (row >= 0 && row < m_stackedWidget->count()) {
        m_stackedWidget->setCurrentIndex(row);
    }
}

void StudentWindow::onRefreshData()
{
    loadCourses();
    loadGrades();
    loadPayments();
}

