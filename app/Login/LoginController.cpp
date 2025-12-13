#include "LoginController.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QDialog>

LoginController::LoginController(QObject* parent)
    : QObject(parent), m_loginWidget(nullptr)
{
}

void LoginController::showLoginDialog()
{
    if (m_loginWidget) {
        m_loginWidget->show();
        return;
    }
    
    QDialog* dialog = new QDialog();
    dialog->setWindowTitle("NCTU SIS - Login");
    dialog->setMinimumWidth(400);
    dialog->setModal(true);
    
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    
    QLabel* titleLabel = new QLabel("نظام معلومات الطلاب - جامعة القاهرة الجديدة التكنولوجية", dialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin: 10px;");
    layout->addWidget(titleLabel);
    
    QLabel* roleLabel = new QLabel("نوع المستخدم:", dialog);
    QComboBox* roleCombo = new QComboBox(dialog);
    roleCombo->addItem("طالب", "student");
    roleCombo->addItem("إداري", "admin");
    roleCombo->addItem("دكتور", "doctor");
    layout->addWidget(roleLabel);
    layout->addWidget(roleCombo);
    
    QLabel* usernameLabel = new QLabel("اسم المستخدم / البريد الإلكتروني:", dialog);
    QLineEdit* usernameEdit = new QLineEdit(dialog);
    usernameEdit->setPlaceholderText("أدخل الرقم الجامعي أو البريد الإلكتروني");
    layout->addWidget(usernameLabel);
    layout->addWidget(usernameEdit);
    
    QLabel* passwordLabel = new QLabel("كلمة المرور:", dialog);
    QLineEdit* passwordEdit = new QLineEdit(dialog);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("أدخل كلمة المرور");
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* loginButton = new QPushButton("تسجيل الدخول", dialog);
    QPushButton* cancelButton = new QPushButton("إلغاء", dialog);
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);
    
    connect(loginButton, &QPushButton::clicked, [=]() {
        QString role = roleCombo->currentData().toString();
        QString username = usernameEdit->text().trimmed();
        QString password = passwordEdit->text();
        
        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(dialog, "خطأ", "يرجى إدخال اسم المستخدم وكلمة المرور");
            return;
        }
        
        if (role == "student") {
            Student student;
            LoginResult result = m_authService.loginStudent(username, password, student);
            
            if (result == LoginResult::Success) {
                emit studentLoggedIn(student);
                dialog->accept();
            } else if (result == LoginResult::MustChangePassword) {
                QMessageBox::information(dialog, "تغيير كلمة المرور", 
                    "يجب تغيير كلمة المرور عند أول تسجيل دخول");
                emit studentLoggedIn(student);
                dialog->accept();
            } else if (result == LoginResult::UserNotFound) {
                QMessageBox::warning(dialog, "خطأ", "اسم المستخدم غير موجود");
            } else if (result == LoginResult::InvalidPassword) {
                QMessageBox::warning(dialog, "خطأ", "كلمة المرور غير صحيحة");
            }
        } else if (role == "admin") {
            User user;
            LoginResult result = m_authService.loginAdmin(username, password, user);
            
            if (result == LoginResult::Success) {
                emit adminLoggedIn(user);
                dialog->accept();
            } else if (result == LoginResult::UserNotFound) {
                QMessageBox::warning(dialog, "خطأ", "البريد الإلكتروني غير موجود");
            } else if (result == LoginResult::InvalidPassword) {
                QMessageBox::warning(dialog, "خطأ", "كلمة المرور غير صحيحة");
            }
        } else if (role == "doctor") {
            User user;
            LoginResult result = m_authService.loginDoctor(username, password, user);
            
            if (result == LoginResult::Success) {
                emit doctorLoggedIn(user);
                dialog->accept();
            } else if (result == LoginResult::UserNotFound) {
                QMessageBox::warning(dialog, "خطأ", "البريد الإلكتروني غير موجود");
            } else if (result == LoginResult::InvalidPassword) {
                QMessageBox::warning(dialog, "خطأ", "كلمة المرور غير صحيحة");
            }
        }
    });
    
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    connect(cancelButton, &QPushButton::clicked, this, &LoginController::loginCancelled);
    
    m_loginWidget = dialog;
    dialog->exec();
}

void LoginController::onStudentLoginClicked()
{
    showLoginDialog();
}

void LoginController::onAdminLoginClicked()
{
    showLoginDialog();
}

void LoginController::onDoctorLoginClicked()
{
    showLoginDialog();
}

