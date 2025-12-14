#include "MainWindow.h"
#include "../Login/LoginController.h"
#include "../AdminWindow/AdminWindow.h"
#include "../StudentWindow/StudentWindow.h"
#include "../DoctorWindow/DoctorWindow.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QCoreApplication>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_stackedWidget(new QStackedWidget(this))
    , m_loginController(new LoginController(this))
    , m_logoLabel(nullptr)
{
    setupUI();
    
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
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // Top area with university logo centered
    QHBoxLayout* logoLayout = new QHBoxLayout;
    m_logoLabel = new QLabel(this);
    m_logoLabel->setAlignment(Qt::AlignCenter);

    // Try to load logo from common locations relative to the executable
    QPixmap logoPixmap;
    const QString appDir = QCoreApplication::applicationDirPath();
    const QStringList candidatePaths = {
        appDir + "/resources/university_logo.png",      // if resources copied next to executable
        appDir + "/../resources/university_logo.png"    // if running from build/ and resources/ is at project root
    };
    for (const QString& path : candidatePaths) {
        if (QFile::exists(path)) {
            logoPixmap.load(path);
            break;
        }
    }

    if (!logoPixmap.isNull()) {
        // Scale nicely while keeping aspect ratio
        m_logoLabel->setPixmap(logoPixmap.scaled(160, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        // Fallback text if image is not found
        m_logoLabel->setText("New Cairo Technological University");
        m_logoLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    }

    logoLayout->addStretch();
    logoLayout->addWidget(m_logoLabel);
    logoLayout->addStretch();

    mainLayout->addLayout(logoLayout);
    mainLayout->addWidget(m_stackedWidget);

    setCentralWidget(central);
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

