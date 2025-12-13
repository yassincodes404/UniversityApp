#include "AdminRepo.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AdminRepo::AdminRepo()
{
}

std::optional<Admin> AdminRepo::findById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM admins WHERE id = ?");
    query.addBindValue(id);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    return mapFromQuery(query);
}

std::optional<Admin> AdminRepo::findByEmail(const QString& email)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM admins WHERE email = ?");
    query.addBindValue(email);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    return mapFromQuery(query);
}

bool AdminRepo::save(Admin& admin)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO admins (name, email, password_hash, salt) VALUES (?, ?, ?, ?)");
    
    query.addBindValue(admin.name);
    query.addBindValue(admin.email);
    query.addBindValue(admin.passwordHash);
    query.addBindValue(admin.salt);
    
    if (!query.exec()) {
        qDebug() << "AdminRepo::save error:" << query.lastError().text();
        return false;
    }
    
    admin.id = query.lastInsertId().toInt();
    return true;
}

bool AdminRepo::update(const Admin& admin)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE admins SET name = ?, email = ?, password_hash = ?, salt = ? WHERE id = ?");
    
    query.addBindValue(admin.name);
    query.addBindValue(admin.email);
    query.addBindValue(admin.passwordHash);
    query.addBindValue(admin.salt);
    query.addBindValue(admin.id);
    
    if (!query.exec()) {
        qDebug() << "AdminRepo::update error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

Admin AdminRepo::mapFromQuery(const QSqlQuery& query)
{
    Admin admin;
    admin.id = query.value("id").toInt();
    admin.name = query.value("name").toString();
    admin.email = query.value("email").toString();
    admin.passwordHash = query.value("password_hash").toString();
    admin.salt = query.value("salt").toString();
    
    return admin;
}

