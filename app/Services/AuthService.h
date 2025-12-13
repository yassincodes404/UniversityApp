#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include "../Models/User.h"
#include "../Models/Student.h"
#include "../DAL/StudentRepo.h"
#include <QString>
#include <optional>

enum class LoginResult {
    Success,
    UserNotFound,
    InvalidPassword,
    MustChangePassword,
    Error
};

class AuthService
{
public:
    AuthService();
    
    LoginResult loginStudent(const QString& studentCode, const QString& password, Student& outStudent);
    LoginResult loginAdmin(const QString& email, const QString& password, User& outUser);
    LoginResult loginDoctor(const QString& email, const QString& password, User& outUser);
    
    bool changePassword(int studentId, const QString& oldPassword, const QString& newPassword);
    
private:
    StudentRepo m_studentRepo;
};

#endif // AUTHSERVICE_H

