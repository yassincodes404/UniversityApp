#include "AuthService.h"
#include "../DAL/StudentRepo.h"
#include "../DAL/AdminRepo.h"
#include "../DAL/DoctorRepo.h"
#include "../Utils/PasswordHasher.h"
#include <QDebug>

LoginResult AuthService::login(UserRole role, const QString& username, const QString& password, LoginInfo& info)
{
    if (role == UserRole::Student) {
        StudentRepo repo;
        auto student = repo.findByCode(username);
        
        if (!student.has_value()) {
            return LoginResult::UserNotFound;
        }
        
        if (!PasswordHasher::verifyPassword(password, student->passwordHash(), student->salt())) {
            return LoginResult::InvalidPassword;
        }
        
        if (student->mustChangePassword()) {
            info.userId = student->id();
            info.role = UserRole::Student;
            info.name = student->name();
            info.email = student->email();
            return LoginResult::MustChangePassword;
        }
        
        info.userId = student->id();
        info.role = UserRole::Student;
        info.name = student->name();
        info.email = student->email();
        return LoginResult::Success;
        
    } else if (role == UserRole::Admin) {
        AdminRepo repo;
        auto admin = repo.findByEmail(username);
        
        if (!admin.has_value()) {
            return LoginResult::UserNotFound;
        }
        
        if (!PasswordHasher::verifyPassword(password, admin->passwordHash, admin->salt)) {
            return LoginResult::InvalidPassword;
        }
        
        info.userId = admin->id;
        info.role = UserRole::Admin;
        info.name = admin->name;
        info.email = admin->email;
        return LoginResult::Success;
        
    } else if (role == UserRole::Doctor) {
        DoctorRepo repo;
        auto doctor = repo.findByEmail(username);
        
        if (!doctor.has_value()) {
            return LoginResult::UserNotFound;
        }
        
        if (!PasswordHasher::verifyPassword(password, doctor->passwordHash(), doctor->salt())) {
            return LoginResult::InvalidPassword;
        }
        
        info.userId = doctor->id();
        info.role = UserRole::Doctor;
        info.name = doctor->name();
        info.email = doctor->email();
        return LoginResult::Success;
    }
    
    return LoginResult::Error;
}

bool AuthService::changePassword(UserRole role, int userId, const QString& oldPassword, const QString& newPassword)
{
    if (role == UserRole::Student) {
        StudentRepo repo;
        auto student = repo.findById(userId);
        if (!student.has_value()) {
            return false;
        }
        
        if (!PasswordHasher::verifyPassword(oldPassword, student->passwordHash(), student->salt())) {
            return false;
        }
        
        QString newSalt = PasswordHasher::generateSalt();
        QString newHash = PasswordHasher::hashPassword(newPassword, newSalt);
        
        student->setPasswordHash(newHash);
        student->setSalt(newSalt);
        student->setMustChangePassword(false);
        
        return repo.update(*student);
        
    } else if (role == UserRole::Admin) {
        AdminRepo repo;
        auto admin = repo.findById(userId);
        if (!admin.has_value()) {
            return false;
        }
        
        if (!PasswordHasher::verifyPassword(oldPassword, admin->passwordHash, admin->salt)) {
            return false;
        }
        
        QString newSalt = PasswordHasher::generateSalt();
        QString newHash = PasswordHasher::hashPassword(newPassword, newSalt);
        
        admin->passwordHash = newHash;
        admin->salt = newSalt;
        
        return repo.update(*admin);
        
    } else if (role == UserRole::Doctor) {
        DoctorRepo repo;
        auto doctor = repo.findById(userId);
        if (!doctor.has_value()) {
            return false;
        }
        
        if (!PasswordHasher::verifyPassword(oldPassword, doctor->passwordHash(), doctor->salt())) {
            return false;
        }
        
        QString newSalt = PasswordHasher::generateSalt();
        QString newHash = PasswordHasher::hashPassword(newPassword, newSalt);
        
        doctor->setPasswordHash(newHash);
        doctor->setSalt(newSalt);
        
        return repo.update(*doctor);
    }
    
    return false;
}

