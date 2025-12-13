#include "MainWindow.h"
#include "../Login/LoginController.h"
#include "../AdminWindow/AdminWindow.h"
#include "../StudentWindow/StudentWindow.h"
#include "../DoctorWindow/DoctorWindow.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_stackedWidget(new QStackedWidget(this))
    , m_loginController(new LoginController(this))
{
    setupUI();
    setCentralWidget(m_stackedWidget);
    
    connect(m_loginController, &LoginController::loginSuccess, 
            this, &MainWindow::onLoginSuccess);
    
    m_stackedWidget->addWidget(m_loginController);
    m_stackedWidget->setCurrentWidget(m_loginController);
    
    setWindowTitle("NCTU Student Information System");
    resize(1000, 700);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
}

void MainWindow::onLoginSuccess(int userId, UserRole role, const QString& name)
{
    QWidget* window = nullptr;
    
    if (role == UserRole::Admin) {
        window = new AdminWindow(userId, this);
    } else if (role == UserRole::Student) {
        window = new StudentWindow(userId, this);
    } else if (role == UserRole::Doctor) {
        window = new DoctorWindow(userId, this);
    }
    
    if (window) {
        // Remove old windows (except login)
        while (m_stackedWidget->count() > 1) {
            QWidget* w = m_stackedWidget->widget(1);
            m_stackedWidget->removeWidget(w);
            w->deleteLater();
        }
        
        m_stackedWidget->addWidget(window);
        m_stackedWidget->setCurrentWidget(window);
        
        // Connect logout signal
        if (role == UserRole::Admin) {
            connect(static_cast<AdminWindow*>(window), &AdminWindow::logoutRequested, 
                    this, &MainWindow::onLogout);
        } else if (role == UserRole::Student) {
            connect(static_cast<StudentWindow*>(window), &StudentWindow::logoutRequested, 
                    this, &MainWindow::onLogout);
        } else if (role == UserRole::Doctor) {
            connect(static_cast<DoctorWindow*>(window), &DoctorWindow::logoutRequested, 
                    this, &MainWindow::onLogout);
        }
    }
}

void MainWindow::onLogout()
{
    // Remove all windows except login
    while (m_stackedWidget->count() > 1) {
        QWidget* w = m_stackedWidget->widget(1);
        m_stackedWidget->removeWidget(w);
        w->deleteLater();
    }
    
    m_stackedWidget->setCurrentWidget(m_loginController);
}

