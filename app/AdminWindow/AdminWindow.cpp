#include "AdminWindow.h"
#include "../DAL/StudentRepo.h"
#include "../DAL/CourseRepo.h"
#include "../DAL/DatabaseManager.h"
#include "../Models/Student.h"
#include "../Models/Course.h"
#include "../Utils/PasswordHasher.h"
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
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDateEdit>
#include <QDialogButtonBox>

// Simple editor dialog for Student
class StudentEditorDialog : public QDialog
{
public:
    explicit StudentEditorDialog(QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setupUi();
    }

    explicit StudentEditorDialog(const Student& student, QWidget* parent = nullptr)
        : QDialog(parent)
        , m_student(student)
    {
        setupUi();
        loadFromStudent();
    }

    Student student() const { return m_student; }

private slots:
    void onAccept()
    {
        if (m_codeEdit->text().trimmed().isEmpty() || m_nameEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation", "Student code and name are required.");
            return;
        }

        Student s = m_student;
        s.setStudentCode(m_codeEdit->text().trimmed());
        s.setName(m_nameEdit->text().trimmed());
        s.setEmail(m_emailEdit->text().trimmed());
        s.setPhone(m_phoneEdit->text().trimmed());
        s.setNationalId(m_nationalIdEdit->text().trimmed());
        s.setBirthDate(m_birthDateEdit->date());
        s.setGender(m_genderCombo->currentText());
        s.setSeatNumber(m_seatNumberEdit->text().trimmed());
        s.setStatus(m_statusCombo->currentText());
        s.setLevel(m_levelSpin->value());
        s.setProgramId(m_programIdSpin->value());
        s.setDepartmentId(m_departmentIdSpin->value());
        s.setCreditsCompleted(m_creditsSpin->value());
        s.setGpa(m_gpaEdit->text().toDouble());

        m_student = s;
        accept();
    }

private:
    void setupUi()
    {
        setWindowTitle("Student Details");
        QFormLayout* form = new QFormLayout(this);

        m_codeEdit = new QLineEdit(this);
        m_nameEdit = new QLineEdit(this);
        m_emailEdit = new QLineEdit(this);
        m_phoneEdit = new QLineEdit(this);
        m_nationalIdEdit = new QLineEdit(this);
        m_birthDateEdit = new QDateEdit(this);
        m_birthDateEdit->setCalendarPopup(true);
        m_birthDateEdit->setDisplayFormat("yyyy-MM-dd");
        m_genderCombo = new QComboBox(this);
        m_genderCombo->addItems({"M", "F", "Other"});
        m_seatNumberEdit = new QLineEdit(this);
        m_statusCombo = new QComboBox(this);
        m_statusCombo->addItems({"active", "graduated", "suspended"});
        m_levelSpin = new QSpinBox(this);
        m_levelSpin->setRange(1, 10);
        m_programIdSpin = new QSpinBox(this);
        m_programIdSpin->setRange(0, 9999);
        m_departmentIdSpin = new QSpinBox(this);
        m_departmentIdSpin->setRange(0, 9999);
        m_creditsSpin = new QSpinBox(this);
        m_creditsSpin->setRange(0, 500);
        m_gpaEdit = new QLineEdit(this);

        form->addRow("Student Code*", m_codeEdit);
        form->addRow("Name*", m_nameEdit);
        form->addRow("Email", m_emailEdit);
        form->addRow("Phone", m_phoneEdit);
        form->addRow("National ID", m_nationalIdEdit);
        form->addRow("Birth Date", m_birthDateEdit);
        form->addRow("Gender", m_genderCombo);
        form->addRow("Seat Number", m_seatNumberEdit);
        form->addRow("Status", m_statusCombo);
        form->addRow("Level", m_levelSpin);
        form->addRow("Program ID", m_programIdSpin);
        form->addRow("Department ID", m_departmentIdSpin);
        form->addRow("Credits Completed", m_creditsSpin);
        form->addRow("GPA", m_gpaEdit);

        QDialogButtonBox* buttons = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
            Qt::Horizontal,
            this
        );
        connect(buttons, &QDialogButtonBox::accepted, this, &StudentEditorDialog::onAccept);
        connect(buttons, &QDialogButtonBox::rejected, this, &StudentEditorDialog::reject);

        form->addRow(buttons);
    }

    void loadFromStudent()
    {
        m_codeEdit->setText(m_student.studentCode());
        m_nameEdit->setText(m_student.name());
        m_emailEdit->setText(m_student.email());
        m_phoneEdit->setText(m_student.phone());
        m_nationalIdEdit->setText(m_student.nationalId());
        if (m_student.birthDate().isValid()) {
            m_birthDateEdit->setDate(m_student.birthDate());
        } else {
            m_birthDateEdit->setDate(QDate::currentDate());
        }
        int genderIndex = m_genderCombo->findText(m_student.gender());
        if (genderIndex >= 0) {
            m_genderCombo->setCurrentIndex(genderIndex);
        }
        m_seatNumberEdit->setText(m_student.seatNumber());
        int statusIndex = m_statusCombo->findText(m_student.status());
        if (statusIndex >= 0) {
            m_statusCombo->setCurrentIndex(statusIndex);
        }
        m_levelSpin->setValue(m_student.level());
        m_programIdSpin->setValue(m_student.programId());
        m_departmentIdSpin->setValue(m_student.departmentId());
        m_creditsSpin->setValue(m_student.creditsCompleted());
        m_gpaEdit->setText(QString::number(m_student.gpa(), 'f', 2));
    }

    Student m_student;
    QLineEdit* m_codeEdit = nullptr;
    QLineEdit* m_nameEdit = nullptr;
    QLineEdit* m_emailEdit = nullptr;
    QLineEdit* m_phoneEdit = nullptr;
    QLineEdit* m_nationalIdEdit = nullptr;
    QDateEdit* m_birthDateEdit = nullptr;
    QComboBox* m_genderCombo = nullptr;
    QLineEdit* m_seatNumberEdit = nullptr;
    QComboBox* m_statusCombo = nullptr;
    QSpinBox* m_levelSpin = nullptr;
    QSpinBox* m_programIdSpin = nullptr;
    QSpinBox* m_departmentIdSpin = nullptr;
    QSpinBox* m_creditsSpin = nullptr;
    QLineEdit* m_gpaEdit = nullptr;
};


// Simple editor dialog for Course
class CourseEditorDialog : public QDialog
{
public:
    explicit CourseEditorDialog(QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setupUi();
        populateDoctors();
    }

    explicit CourseEditorDialog(const Course& course, int doctorId, QWidget* parent = nullptr)
        : QDialog(parent)
        , m_course(course)
        , m_doctorId(doctorId)
    {
        setupUi();
        populateDoctors();
        loadFromCourse();
    }

    Course course() const { return m_course; }
    int doctorId() const { return m_doctorId; }

private slots:
    void onAccept()
    {
        if (m_codeEdit->text().trimmed().isEmpty() || m_nameEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation", "Course code and name are required.");
            return;
        }

        Course c = m_course;
        c.setCode(m_codeEdit->text().trimmed());
        c.setName(m_nameEdit->text().trimmed());
        c.setCreditHours(m_creditHoursSpin->value());
        c.setProgramId(m_programIdSpin->value());
        c.setYear(m_yearSpin->value());
        c.setSemester(m_semesterCombo->currentText());
        c.setCourseType(m_courseTypeCombo->currentText());
        c.setDescription(m_descriptionEdit->text().trimmed());
        c.setPrerequisites(m_prereqEdit->text().trimmed());

        m_course = c;

        QVariant doctorData = m_doctorCombo->currentData();
        m_doctorId = doctorData.isValid() ? doctorData.toInt() : 0;

        accept();
    }

private:
    void setupUi()
    {
        setWindowTitle("Course Details");
        QFormLayout* form = new QFormLayout(this);

        m_codeEdit = new QLineEdit(this);
        m_nameEdit = new QLineEdit(this);
        m_creditHoursSpin = new QSpinBox(this);
        m_creditHoursSpin->setRange(1, 10);
        m_programIdSpin = new QSpinBox(this);
        m_programIdSpin->setRange(0, 9999);
        m_yearSpin = new QSpinBox(this);
        m_yearSpin->setRange(1, 6);
        m_semesterCombo = new QComboBox(this);
        m_semesterCombo->addItems({"Fall", "Spring"});
        m_courseTypeCombo = new QComboBox(this);
        m_courseTypeCombo->addItems({"Theory", "Practical", "Mixed"});
        m_descriptionEdit = new QLineEdit(this);
        m_prereqEdit = new QLineEdit(this);
        m_doctorCombo = new QComboBox(this);
        m_doctorCombo->setEditable(false);

        form->addRow("Code*", m_codeEdit);
        form->addRow("Name*", m_nameEdit);
        form->addRow("Credit Hours", m_creditHoursSpin);
        form->addRow("Program ID", m_programIdSpin);
        form->addRow("Year", m_yearSpin);
        form->addRow("Semester", m_semesterCombo);
        form->addRow("Course Type", m_courseTypeCombo);
        form->addRow("Description", m_descriptionEdit);
        form->addRow("Prerequisites", m_prereqEdit);
        form->addRow("Doctor", m_doctorCombo);

        QDialogButtonBox* buttons = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
            Qt::Horizontal,
            this
        );
        connect(buttons, &QDialogButtonBox::accepted, this, &CourseEditorDialog::onAccept);
        connect(buttons, &QDialogButtonBox::rejected, this, &CourseEditorDialog::reject);

        form->addRow(buttons);
    }

    void populateDoctors()
    {
        m_doctorCombo->clear();
        m_doctorCombo->addItem("No doctor assigned", 0);

        QSqlQuery query(DatabaseManager::instance().database());
        // Using old schema doctors table (id, name, email, ...)
        if (query.exec("SELECT id, name, email FROM doctors ORDER BY name")) {
            while (query.next()) {
                int id = query.value(0).toInt();
                QString name = query.value(1).toString();
                QString email = query.value(2).toString();
                QString label = name;
                if (!email.isEmpty()) {
                    label += " (" + email + ")";
                }
                m_doctorCombo->addItem(label, id);
            }
        }

        if (m_doctorId > 0) {
            for (int i = 0; i < m_doctorCombo->count(); ++i) {
                if (m_doctorCombo->itemData(i).toInt() == m_doctorId) {
                    m_doctorCombo->setCurrentIndex(i);
                    break;
                }
            }
        }
    }

    void loadFromCourse()
    {
        m_codeEdit->setText(m_course.code());
        m_nameEdit->setText(m_course.name());
        m_creditHoursSpin->setValue(m_course.creditHours());
        m_programIdSpin->setValue(m_course.programId());
        m_yearSpin->setValue(m_course.year());
        int semIndex = m_semesterCombo->findText(m_course.semester());
        if (semIndex >= 0) {
            m_semesterCombo->setCurrentIndex(semIndex);
        }
        int typeIndex = m_courseTypeCombo->findText(m_course.courseType());
        if (typeIndex >= 0) {
            m_courseTypeCombo->setCurrentIndex(typeIndex);
        }
        m_descriptionEdit->setText(m_course.description());
        m_prereqEdit->setText(m_course.prerequisites());
    }

    Course m_course;
    int m_doctorId = 0;
    QLineEdit* m_codeEdit = nullptr;
    QLineEdit* m_nameEdit = nullptr;
    QSpinBox* m_creditHoursSpin = nullptr;
    QSpinBox* m_programIdSpin = nullptr;
    QSpinBox* m_yearSpin = nullptr;
    QComboBox* m_semesterCombo = nullptr;
    QComboBox* m_courseTypeCombo = nullptr;
    QLineEdit* m_descriptionEdit = nullptr;
    QLineEdit* m_prereqEdit = nullptr;
    QComboBox* m_doctorCombo = nullptr;
};


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

    // Students search
    QHBoxLayout* studentsSearchLayout = new QHBoxLayout;
    m_studentSearchEdit = new QLineEdit(this);
    m_studentSearchEdit->setPlaceholderText("Search students by code, name, or email...");
    connect(m_studentSearchEdit, &QLineEdit::textChanged,
            this, &AdminWindow::refreshStudents);
    studentsSearchLayout->addWidget(m_studentSearchEdit);
    
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
    studentsLayout->addLayout(studentsSearchLayout);
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

    // Courses search
    QHBoxLayout* coursesSearchLayout = new QHBoxLayout;
    m_courseSearchEdit = new QLineEdit(this);
    m_courseSearchEdit->setPlaceholderText("Search courses by code or name...");
    connect(m_courseSearchEdit, &QLineEdit::textChanged,
            this, &AdminWindow::refreshCourses);
    coursesSearchLayout->addWidget(m_courseSearchEdit);
    
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
    coursesLayout->addLayout(coursesSearchLayout);
    coursesLayout->addWidget(m_coursesTable);
    
    m_tabs->addTab(coursesWidget, "Courses");

    // Profile image requests tab
    QWidget* imageReqWidget = new QWidget;
    QVBoxLayout* imageReqLayout = new QVBoxLayout(imageReqWidget);

    QHBoxLayout* imageReqButtons = new QHBoxLayout;
    QPushButton* approveBtn = new QPushButton("Approve", this);
    QPushButton* rejectBtn = new QPushButton("Reject", this);
    QPushButton* refreshImageReqBtn = new QPushButton("Refresh", this);
    connect(approveBtn, &QPushButton::clicked, this, &AdminWindow::onApproveProfileImage);
    connect(rejectBtn, &QPushButton::clicked, this, &AdminWindow::onRejectProfileImage);
    connect(refreshImageReqBtn, &QPushButton::clicked, this, &AdminWindow::refreshProfileImageRequests);
    imageReqButtons->addWidget(approveBtn);
    imageReqButtons->addWidget(rejectBtn);
    imageReqButtons->addStretch();
    imageReqButtons->addWidget(refreshImageReqBtn);

    m_profileImageRequestsTable = new QTableView(this);
    QSqlTableModel* imageReqModel = new QSqlTableModel(m_profileImageRequestsTable, DatabaseManager::instance().database());
    imageReqModel->setTable("profile_image_change_requests");
    imageReqModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    imageReqModel->setFilter("status = 'pending'");
    imageReqModel->select();
    imageReqModel->setHeaderData(1, Qt::Horizontal, "Student ID");
    imageReqModel->setHeaderData(2, Qt::Horizontal, "Old Image Path");
    imageReqModel->setHeaderData(3, Qt::Horizontal, "New Image Path");
    imageReqModel->setHeaderData(4, Qt::Horizontal, "Status");
    imageReqModel->setHeaderData(5, Qt::Horizontal, "Requested At");

    m_profileImageRequestsTable->setModel(imageReqModel);
    m_profileImageRequestsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_profileImageRequestsTable->horizontalHeader()->setStretchLastSection(true);
    m_profileImageRequestsTable->resizeColumnsToContents();

    imageReqLayout->addLayout(imageReqButtons);
    imageReqLayout->addWidget(m_profileImageRequestsTable);

    m_tabs->addTab(imageReqWidget, "Profile Images");

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
    StudentEditorDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    Student student = dlg.student();

    // For new students, generate default password hash/salt
    const QString defaultPassword = "student123";
    const QString salt = PasswordHasher::generateSalt();
    const QString hash = PasswordHasher::hashPassword(defaultPassword, salt);
    student.setSalt(salt);
    student.setPasswordHash(hash);
    student.setMustChangePassword(true);

    StudentRepo repo;
    if (!repo.save(student)) {
        QMessageBox::critical(this, "Error", "Failed to save student to database.");
        return;
    }

    refreshStudents();
}

void AdminWindow::onEditStudent()
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(m_studentsTable->model());
    if (!model) {
        return;
    }
    QModelIndex index = m_studentsTable->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Edit Student", "Please select a student to edit.");
        return;
    }

    int row = index.row();
    int id = model->data(model->index(row, 0)).toInt(); // id column

    StudentRepo repo;
    auto existingOpt = repo.findById(id);
    if (!existingOpt.has_value()) {
        QMessageBox::critical(this, "Error", "Could not load selected student from database.");
        return;
    }

    Student existing = existingOpt.value();
    StudentEditorDialog dlg(existing, this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    Student updated = dlg.student();
    // Preserve password fields unless we later add UI for them
    updated.setPasswordHash(existing.passwordHash());
    updated.setSalt(existing.salt());
    updated.setMustChangePassword(existing.mustChangePassword());

    if (!repo.update(updated)) {
        QMessageBox::critical(this, "Error", "Failed to update student in database.");
        return;
    }

    refreshStudents();
}

void AdminWindow::onDeleteStudent()
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(m_studentsTable->model());
    if (!model) {
        return;
    }
    QModelIndex index = m_studentsTable->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Delete Student", "Please select a student to delete.");
        return;
    }

    int row = index.row();
    int id = model->data(model->index(row, 0)).toInt();

    if (QMessageBox::question(
            this,
            "Delete Student",
            "Are you sure you want to delete the selected student?"
        ) != QMessageBox::Yes) {
        return;
    }

    StudentRepo repo;
    if (!repo.remove(id)) {
        QMessageBox::critical(this, "Error", "Failed to delete student from database.");
        return;
    }

    refreshStudents();
}

void AdminWindow::onAddCourse()
{
    CourseEditorDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    Course course = dlg.course();
    CourseRepo repo;
    if (!repo.save(course)) {
        QMessageBox::critical(this, "Error", "Failed to save course to database.");
        return;
    }

    // After creating a course, optionally create or update a section with assigned doctor
    int doctorId = dlg.doctorId();
    if (doctorId > 0) {
        QSqlQuery sectionQuery(DatabaseManager::instance().database());
        sectionQuery.prepare(
            "INSERT INTO sections (section_type, course_id, doctor_id) "
            "VALUES ('Lecture', ?, ?)"
        );
        sectionQuery.addBindValue(course.id());
        sectionQuery.addBindValue(doctorId);
        if (!sectionQuery.exec()) {
            qDebug() << "Failed to create section for course:" << sectionQuery.lastError().text();
        }
    }

    refreshCourses();
}

void AdminWindow::onEditCourse()
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(m_coursesTable->model());
    if (!model) {
        return;
    }
    QModelIndex index = m_coursesTable->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Edit Course", "Please select a course to edit.");
        return;
    }

    int row = index.row();
    int id = model->data(model->index(row, 0)).toInt();

    CourseRepo repo;
    auto existingOpt = repo.findById(id);
    if (!existingOpt.has_value()) {
        QMessageBox::critical(this, "Error", "Could not load selected course from database.");
        return;
    }

    Course existing = existingOpt.value();

    // Determine currently assigned doctor via sections table (if any)
    int currentDoctorId = 0;
    {
        QSqlQuery query(DatabaseManager::instance().database());
        query.prepare("SELECT doctor_id FROM sections WHERE course_id = ? LIMIT 1");
        query.addBindValue(existing.id());
        if (query.exec() && query.next()) {
            currentDoctorId = query.value(0).toInt();
        }
    }

    CourseEditorDialog dlg(existing, currentDoctorId, this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    Course updated = dlg.course();
    updated.setId(existing.id()); // keep id

    if (!repo.update(updated)) {
        QMessageBox::critical(this, "Error", "Failed to update course in database.");
        return;
    }

    // Update or create section assignment for selected doctor
    int doctorId = dlg.doctorId();
    QSqlQuery sectionQuery(DatabaseManager::instance().database());

    int existingSectionId = 0;
    {
        QSqlQuery findSection(DatabaseManager::instance().database());
        findSection.prepare("SELECT id FROM sections WHERE course_id = ? LIMIT 1");
        findSection.addBindValue(existing.id());
        if (findSection.exec() && findSection.next()) {
            existingSectionId = findSection.value(0).toInt();
        }
    }

    if (doctorId > 0) {
        if (existingSectionId > 0) {
            sectionQuery.prepare("UPDATE sections SET doctor_id = ? WHERE id = ?");
            sectionQuery.addBindValue(doctorId);
            sectionQuery.addBindValue(existingSectionId);
        } else {
            sectionQuery.prepare(
                "INSERT INTO sections (section_type, course_id, doctor_id) "
                "VALUES ('Lecture', ?, ?)"
            );
            sectionQuery.addBindValue(existing.id());
            sectionQuery.addBindValue(doctorId);
        }
        if (!sectionQuery.exec()) {
            qDebug() << "Failed to update section for course:" << sectionQuery.lastError().text();
        }
    } else if (existingSectionId > 0) {
        // No doctor selected now: clear doctor assignment
        sectionQuery.prepare("UPDATE sections SET doctor_id = NULL WHERE id = ?");
        sectionQuery.addBindValue(existingSectionId);
        if (!sectionQuery.exec()) {
            qDebug() << "Failed to clear doctor assignment for course:" << sectionQuery.lastError().text();
        }
    }

    refreshCourses();
}

void AdminWindow::onDeleteCourse()
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(m_coursesTable->model());
    if (!model) {
        return;
    }
    QModelIndex index = m_coursesTable->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Delete Course", "Please select a course to delete.");
        return;
    }

    int row = index.row();
    int id = model->data(model->index(row, 0)).toInt();

    if (QMessageBox::question(
            this,
            "Delete Course",
            "Are you sure you want to delete the selected course?"
        ) != QMessageBox::Yes) {
        return;
    }

    CourseRepo repo;
    if (!repo.remove(id)) {
        QMessageBox::critical(this, "Error", "Failed to delete course from database.");
        return;
    }

    refreshCourses();
}

void AdminWindow::refreshStudents()
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(m_studentsTable->model());
    if (model) {
        QString filterText = m_studentSearchEdit ? m_studentSearchEdit->text().trimmed() : QString();
        if (filterText.isEmpty()) {
            model->setFilter(QString());
        } else {
            QString pattern = "%" + filterText.replace("'", "''") + "%";
            model->setFilter(
                QString("student_code LIKE '%1' OR name LIKE '%1' OR email LIKE '%1'")
                    .arg(pattern)
            );
        }
        model->select();
    }
}

void AdminWindow::refreshCourses()
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(m_coursesTable->model());
    if (model) {
        QString filterText = m_courseSearchEdit ? m_courseSearchEdit->text().trimmed() : QString();
        if (filterText.isEmpty()) {
            model->setFilter(QString());
        } else {
            QString pattern = "%" + filterText.replace("'", "''") + "%";
            model->setFilter(
                QString("code LIKE '%1' OR name LIKE '%1'").arg(pattern)
            );
        }
        model->select();
    }
}

void AdminWindow::refreshProfileImageRequests()
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(m_profileImageRequestsTable->model());
    if (model) {
        model->setFilter("status = 'pending'");
        model->select();
    }
}

void AdminWindow::onApproveProfileImage()
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(m_profileImageRequestsTable->model());
    if (!model) {
        return;
    }

    QModelIndex index = m_profileImageRequestsTable->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Approve Profile Image", "Please select a request to approve.");
        return;
    }

    int row = index.row();
    int requestId = model->data(model->index(row, 0)).toInt();       // id
    int studentId = model->data(model->index(row, 1)).toInt();       // student_id
    QString newImagePath = model->data(model->index(row, 3)).toString(); // new_image_path

    QSqlDatabase db = DatabaseManager::instance().database();
    QSqlQuery query(db);

    // Update student's profile image path
    query.prepare("UPDATE students SET profile_image_path = ? WHERE id = ?");
    query.addBindValue(newImagePath);
    query.addBindValue(studentId);
    if (!query.exec()) {
        QMessageBox::critical(this, "Approve Profile Image",
                              "Failed to update student's profile image.");
        return;
    }

    // Update request status
    QSqlQuery reqQuery(db);
    reqQuery.prepare(
        "UPDATE profile_image_change_requests "
        "SET status = 'approved', reviewed_at = CURRENT_TIMESTAMP, reviewed_by_admin_id = ? "
        "WHERE id = ?"
    );
    reqQuery.addBindValue(m_adminId);
    reqQuery.addBindValue(requestId);
    if (!reqQuery.exec()) {
        QMessageBox::critical(this, "Approve Profile Image",
                              "Failed to update profile image request status.");
        return;
    }

    refreshProfileImageRequests();
}

void AdminWindow::onRejectProfileImage()
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(m_profileImageRequestsTable->model());
    if (!model) {
        return;
    }

    QModelIndex index = m_profileImageRequestsTable->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Reject Profile Image", "Please select a request to reject.");
        return;
    }

    int row = index.row();
    int requestId = model->data(model->index(row, 0)).toInt(); // id

    QSqlDatabase db = DatabaseManager::instance().database();
    QSqlQuery reqQuery(db);
    reqQuery.prepare(
        "UPDATE profile_image_change_requests "
        "SET status = 'rejected', reviewed_at = CURRENT_TIMESTAMP, reviewed_by_admin_id = ? "
        "WHERE id = ?"
    );
    reqQuery.addBindValue(m_adminId);
    reqQuery.addBindValue(requestId);
    if (!reqQuery.exec()) {
        QMessageBox::critical(this, "Reject Profile Image",
                              "Failed to update profile image request status.");
        return;
    }

    refreshProfileImageRequests();
}

