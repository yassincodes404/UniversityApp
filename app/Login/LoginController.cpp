#include "LoginController.h"
#include "../Services/AuthService.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>
#include <QGroupBox>

LoginController::LoginController(QWidget *parent)
    : QWidget(parent)
    , m_roleCombo(new QComboBox(this))
    , m_usernameEdit(new QLineEdit(this))
    , m_passwordEdit(new QLineEdit(this))
    , m_loginButton(new QPushButton("Login", this))
    , m_statusLabel(new QLabel(this))
{
    setupUI();
    
    connect(m_loginButton, &QPushButton::clicked, this, &LoginController::onLoginClicked);
    connect(m_roleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &LoginController::onRoleChanged);
    
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_statusLabel->setStyleSheet("color: red;");
    m_statusLabel->setVisible(false);
}

void LoginController::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QGroupBox* loginBox = new QGroupBox("Login", this);
    QFormLayout* formLayout = new QFormLayout(loginBox);
    
    m_roleCombo->addItems({"Student", "Admin", "Doctor"});
    formLayout->addRow("Role:", m_roleCombo);
    
    m_usernameEdit->setPlaceholderText("Enter username/email");
    formLayout->addRow("Username:", m_usernameEdit);
    
    m_passwordEdit->setPlaceholderText("Enter password");
    formLayout->addRow("Password:", m_passwordEdit);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_loginButton);
    buttonLayout->addStretch();
    
    formLayout->addRow(buttonLayout);
    formLayout->addRow(m_statusLabel);
    
    mainLayout->addStretch();
    mainLayout->addWidget(loginBox, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    
    setLayout(mainLayout);
}

void LoginController::onLoginClicked()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        m_statusLabel->setText("Please enter username and password");
        m_statusLabel->setVisible(true);
        return;
    }
    
    UserRole role = static_cast<UserRole>(m_roleCombo->currentIndex());
    LoginInfo info;
    LoginResult result = AuthService::login(role, username, password, info);
    
    if (result == LoginResult::Success) {
        m_statusLabel->setVisible(false);
        emit loginSuccess(info.userId, info.role, info.name);
    } else if (result == LoginResult::MustChangePassword) {
        QMessageBox::information(this, "Change Password", 
                                "You must change your password on first login.");
        // TODO: Open password change dialog
        emit loginSuccess(info.userId, info.role, info.name);
    } else if (result == LoginResult::UserNotFound) {
        m_statusLabel->setText("User not found");
        m_statusLabel->setVisible(true);
    } else if (result == LoginResult::InvalidPassword) {
        m_statusLabel->setText("Invalid password");
        m_statusLabel->setVisible(true);
    } else {
        m_statusLabel->setText("Login failed. Please try again.");
        m_statusLabel->setVisible(true);
    }
}

void LoginController::onRoleChanged(int index)
{
    Q_UNUSED(index);
    m_statusLabel->setVisible(false);
    m_usernameEdit->clear();
    m_passwordEdit->clear();
}

