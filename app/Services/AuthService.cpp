#include "AuthService.h"
#include "../Utils/PasswordHasher.h"
#include "../DAL/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AuthService::AuthService()
{
}

LoginResult AuthService::loginStudent(const QString& studentCode, const QString& password, Student& outStudent)
{
    auto student = m_studentRepo.findByCode(studentCode);
    if (!student.has_value()) {
        return LoginResult::UserNotFound;
    }
    
    if (!PasswordHasher::verifyPassword(password, student->passwordHash, student->salt)) {
        return LoginResult::InvalidPassword;
    }
    
    if (student->mustChangePassword) {
        outStudent = student.value();
        return LoginResult::MustChangePassword;
    }
    
    outStudent = student.value();
    return LoginResult::Success;
}

LoginResult AuthService::loginAdmin(const QString& email, const QString& password, User& outUser)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, name, email, password_hash, salt FROM admins WHERE email = :email");
    query.bindValue(":email", email);
    
    if (!query.exec()) {
        qDebug() << "AuthService::loginAdmin error:" << query.lastError().text();
        return LoginResult::Error;
    }
    
    if (!query.next()) {
        return LoginResult::UserNotFound;
    }
    
    QString hash = query.value(3).toString();
    QString salt = query.value(4).toString();
    
    if (!PasswordHasher::verifyPassword(password, hash, salt)) {
        return LoginResult::InvalidPassword;
    }
    
    outUser.id = query.value(0).toInt();
    outUser.name = query.value(1).toString();
    outUser.email = query.value(2).toString();
    outUser.role = UserRole::Admin;
    
    return LoginResult::Success;
}

LoginResult AuthService::loginDoctor(const QString& email, const QString& password, User& outUser)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, name, email, password_hash, salt FROM doctors WHERE email = :email");
    query.bindValue(":email", email);
    
    if (!query.exec()) {
        qDebug() << "AuthService::loginDoctor error:" << query.lastError().text();
        return LoginResult::Error;
    }
    
    if (!query.next()) {
        return LoginResult::UserNotFound;
    }
    
    QString hash = query.value(3).toString();
    QString salt = query.value(4).toString();
    
    if (!PasswordHasher::verifyPassword(password, hash, salt)) {
        return LoginResult::InvalidPassword;
    }
    
    outUser.id = query.value(0).toInt();
    outUser.name = query.value(1).toString();
    outUser.email = query.value(2).toString();
    outUser.role = UserRole::Doctor;
    
    return LoginResult::Success;
}

bool AuthService::changePassword(int studentId, const QString& oldPassword, const QString& newPassword)
{
    auto student = m_studentRepo.findById(studentId);
    if (!student.has_value()) {
        return false;
    }
    
    if (!PasswordHasher::verifyPassword(oldPassword, student->passwordHash, student->salt)) {
        return false;
    }
    
    QString newSalt = PasswordHasher::generateSalt();
    QString newHash = PasswordHasher::hashPassword(newPassword, newSalt);
    
    return m_studentRepo.updatePassword(studentId, newHash, newSalt);
}

