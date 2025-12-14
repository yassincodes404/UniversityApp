#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../Login/LoginController.h"

QT_BEGIN_NAMESPACE
class QStackedWidget;
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginSuccess(int userId, UserRole role, const QString& name);
    void onLogout();

private:
    void setupUI();
    
    QStackedWidget* m_stackedWidget;
    LoginController* m_loginController;
    QLabel* m_logoLabel;
};

#endif // MAINWINDOW_H

