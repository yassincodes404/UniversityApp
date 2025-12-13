#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include "../Models/User.h"
#include "../Models/Student.h"
#include "../Services/AuthService.h"
#include <QObject>
#include <QWidget>

class LoginController : public QObject
{
    Q_OBJECT

public:
    explicit LoginController(QObject* parent = nullptr);
    
    void showLoginDialog();
    
signals:
    void studentLoggedIn(const Student& student);
    void adminLoggedIn(const User& user);
    void doctorLoggedIn(const User& user);
    void loginCancelled();

private slots:
    void onStudentLoginClicked();
    void onAdminLoginClicked();
    void onDoctorLoginClicked();

private:
    AuthService m_authService;
    QWidget* m_loginWidget;
};

#endif // LOGINCONTROLLER_H

