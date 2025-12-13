#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QString>

enum class LoginResult {
    Success,
    UserNotFound,
    InvalidPassword,
    MustChangePassword,
    Error
};

enum class UserRole {
    Student,
    Admin,
    Doctor
};

struct LoginInfo {
    int userId = 0;
    UserRole role = UserRole::Student;
    QString name;
    QString email;
};

class AuthService
{
public:
    static LoginResult login(UserRole role, const QString& username, const QString& password, LoginInfo& info);
    static bool changePassword(UserRole role, int userId, const QString& oldPassword, const QString& newPassword);
};

#endif // AUTHSERVICE_H

