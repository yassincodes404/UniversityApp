#ifndef USER_H
#define USER_H

#include <QString>

enum class UserRole {
    Student,
    Admin,
    Doctor
};

struct User {
    int id;
    QString name;
    QString email;
    UserRole role;
    
    User() : id(0), role(UserRole::Student) {}
};

#endif // USER_H

