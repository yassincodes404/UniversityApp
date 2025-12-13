#include <QApplication>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QObject>

#include "DAL/DatabaseManager.h"
#include "Login/LoginController.h"
#include "AdminWindow/AdminWindow.h"
#include "StudentWindow/StudentWindow.h"
#include "DoctorWindow/DoctorWindow.h"
#include "Models/User.h"
#include "Models/Student.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("NCTU SIS");
    app.setOrganizationName("NCTU");
    
    // Database configuration dialog
    QDialog configDialog;
    configDialog.setWindowTitle("إعدادات قاعدة البيانات");
    configDialog.setMinimumWidth(400);
    
    QVBoxLayout* configLayout = new QVBoxLayout(&configDialog);
    
    QLabel* titleLabel = new QLabel("إعدادات الاتصال بقاعدة البيانات", &configDialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin: 10px;");
    configLayout->addWidget(titleLabel);
    
    // Try to detect Windows host IP for WSL
    QString defaultHost = "localhost";
    QFile resolvFile("/etc/resolv.conf");
    if (resolvFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&resolvFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith("nameserver")) {
                QString ip = line.split(QRegExp("\\s+")).value(1);
                if (!ip.isEmpty() && ip != "127.0.0.1") {
                    defaultHost = ip;
                    break;
                }
            }
        }
        resolvFile.close();
    }
    QLineEdit* hostEdit = new QLineEdit(defaultHost, &configDialog);
    QLineEdit* dbNameEdit = new QLineEdit("nctu_sis", &configDialog);
    QLineEdit* userEdit = new QLineEdit("root", &configDialog);
    QLineEdit* passEdit = new QLineEdit(&configDialog);
    passEdit->setEchoMode(QLineEdit::Password);
    QLineEdit* portEdit = new QLineEdit("3306", &configDialog);
    
    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("المضيف:", hostEdit);
    formLayout->addRow("اسم قاعدة البيانات:", dbNameEdit);
    formLayout->addRow("اسم المستخدم:", userEdit);
    formLayout->addRow("كلمة المرور:", passEdit);
    formLayout->addRow("المنفذ:", portEdit);
    
    configLayout->addLayout(formLayout);
    
    QPushButton* connectBtn = new QPushButton("اتصال", &configDialog);
    QPushButton* cancelBtn = new QPushButton("إلغاء", &configDialog);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(connectBtn);
    btnLayout->addWidget(cancelBtn);
    configLayout->addLayout(btnLayout);
    
    bool connected = false;
    
    QObject::connect(connectBtn, &QPushButton::clicked, [&]() {
        DbConfig config;
        config.hostName = hostEdit->text();
        config.databaseName = dbNameEdit->text();
        config.userName = userEdit->text();
        config.password = passEdit->text();
        config.port = portEdit->text().toInt();
        
        if (DatabaseManager::instance().open(config)) {
            connected = true;
            configDialog.accept();
        } else {
            QMessageBox::critical(&configDialog, "خطأ", 
                "فشل الاتصال بقاعدة البيانات.\nتأكد من:\n"
                "- تشغيل MySQL\n"
                "- صحة بيانات الاتصال\n"
                "- وجود قاعدة البيانات nctu_sis");
        }
    });
    
    QObject::connect(cancelBtn, &QPushButton::clicked, &configDialog, &QDialog::reject);
    
    if (configDialog.exec() != QDialog::Accepted || !connected) {
        return 1;
    }
    
    // Show login
    LoginController loginController;
    QWidget* mainWindow = nullptr;
    
    QObject::connect(&loginController, &LoginController::studentLoggedIn, 
                     [&](const Student& student) {
        if (mainWindow) {
            mainWindow->close();
        }
        mainWindow = new StudentWindow(student);
        mainWindow->show();
    });
    
    QObject::connect(&loginController, &LoginController::adminLoggedIn, 
                     [&](const User& user) {
        if (mainWindow) {
            mainWindow->close();
        }
        mainWindow = new AdminWindow(user);
        mainWindow->show();
    });
    
    QObject::connect(&loginController, &LoginController::doctorLoggedIn, 
                     [&](const User& user) {
        if (mainWindow) {
            mainWindow->close();
        }
        mainWindow = new DoctorWindow(user);
        mainWindow->show();
    });
    
    loginController.showLoginDialog();
    
    return app.exec();
}

