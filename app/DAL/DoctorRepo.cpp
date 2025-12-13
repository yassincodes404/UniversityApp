#include "DoctorRepo.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DoctorRepo::DoctorRepo()
{
}

std::optional<Doctor> DoctorRepo::findById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM doctors WHERE id = ?");
    query.addBindValue(id);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    return mapFromQuery(query);
}

std::optional<Doctor> DoctorRepo::findByEmail(const QString& email)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM doctors WHERE email = ?");
    query.addBindValue(email);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    return mapFromQuery(query);
}

QList<Doctor> DoctorRepo::findAll()
{
    QList<Doctor> doctors;
    QSqlQuery query("SELECT * FROM doctors ORDER BY name", DatabaseManager::instance().database());
    
    while (query.next()) {
        doctors.append(mapFromQuery(query));
    }
    
    return doctors;
}

QList<Doctor> DoctorRepo::findByDepartment(int departmentId)
{
    QList<Doctor> doctors;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM doctors WHERE department_id = ? ORDER BY name");
    query.addBindValue(departmentId);
    
    if (query.exec()) {
        while (query.next()) {
            doctors.append(mapFromQuery(query));
        }
    }
    
    return doctors;
}

bool DoctorRepo::save(Doctor& doctor)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO doctors (name, email, phone, password_hash, salt, department_id) "
        "VALUES (?, ?, ?, ?, ?, ?)"
    );
    
    query.addBindValue(doctor.name());
    query.addBindValue(doctor.email());
    query.addBindValue(doctor.phone());
    query.addBindValue(doctor.passwordHash());
    query.addBindValue(doctor.salt());
    query.addBindValue(doctor.departmentId());
    
    if (!query.exec()) {
        qDebug() << "DoctorRepo::save error:" << query.lastError().text();
        return false;
    }
    
    doctor.setId(query.lastInsertId().toInt());
    return true;
}

bool DoctorRepo::update(const Doctor& doctor)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "UPDATE doctors SET name = ?, email = ?, phone = ?, password_hash = ?, "
        "salt = ?, department_id = ? WHERE id = ?"
    );
    
    query.addBindValue(doctor.name());
    query.addBindValue(doctor.email());
    query.addBindValue(doctor.phone());
    query.addBindValue(doctor.passwordHash());
    query.addBindValue(doctor.salt());
    query.addBindValue(doctor.departmentId());
    query.addBindValue(doctor.id());
    
    if (!query.exec()) {
        qDebug() << "DoctorRepo::update error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool DoctorRepo::remove(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM doctors WHERE id = ?");
    query.addBindValue(id);
    
    return query.exec();
}

Doctor DoctorRepo::mapFromQuery(const QSqlQuery& query)
{
    Doctor doctor;
    doctor.setId(query.value("id").toInt());
    doctor.setName(query.value("name").toString());
    doctor.setEmail(query.value("email").toString());
    doctor.setPhone(query.value("phone").toString());
    doctor.setPasswordHash(query.value("password_hash").toString());
    doctor.setSalt(query.value("salt").toString());
    doctor.setDepartmentId(query.value("department_id").toInt());
    
    return doctor;
}

