#include "StudentRepo.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

StudentRepo::StudentRepo()
{
}

std::optional<Student> StudentRepo::findByCode(const QString& code)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, student_code, name, email, phone, password_hash, salt, "
                  "program_id, department_id, national_id, birth_date, gender, seat_number, "
                  "status, level, credits_completed, gpa, must_change_password "
                  "FROM students WHERE student_code = :code");
    query.bindValue(":code", code);
    
    if (!query.exec()) {
        qDebug() << "StudentRepo::findByCode error:" << query.lastError().text();
        return std::nullopt;
    }
    
    if (query.next()) {
        Student student;
        student.id = query.value(0).toInt();
        student.studentCode = query.value(1).toString();
        student.name = query.value(2).toString();
        student.email = query.value(3).toString();
        student.phone = query.value(4).toString();
        student.passwordHash = query.value(5).toString();
        student.salt = query.value(6).toString();
        student.programId = query.value(7).toInt();
        student.departmentId = query.value(8).toInt();
        student.nationalId = query.value(9).toString();
        student.birthDate = query.value(10).toDate();
        student.gender = Student::genderFromString(query.value(11).toString());
        student.seatNumber = query.value(12).toString();
        student.status = Student::statusFromString(query.value(13).toString());
        student.level = query.value(14).toInt();
        student.creditsCompleted = query.value(15).toInt();
        student.gpa = query.value(16).toDouble();
        student.mustChangePassword = query.value(17).toBool();
        return student;
    }
    
    return std::nullopt;
}

std::optional<Student> StudentRepo::findById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, student_code, name, email, phone, password_hash, salt, "
                  "program_id, department_id, national_id, birth_date, gender, seat_number, "
                  "status, level, credits_completed, gpa, must_change_password "
                  "FROM students WHERE id = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qDebug() << "StudentRepo::findById error:" << query.lastError().text();
        return std::nullopt;
    }
    
    if (query.next()) {
        Student student;
        student.id = query.value(0).toInt();
        student.studentCode = query.value(1).toString();
        student.name = query.value(2).toString();
        student.email = query.value(3).toString();
        student.phone = query.value(4).toString();
        student.passwordHash = query.value(5).toString();
        student.salt = query.value(6).toString();
        student.programId = query.value(7).toInt();
        student.departmentId = query.value(8).toInt();
        student.nationalId = query.value(9).toString();
        student.birthDate = query.value(10).toDate();
        student.gender = Student::genderFromString(query.value(11).toString());
        student.seatNumber = query.value(12).toString();
        student.status = Student::statusFromString(query.value(13).toString());
        student.level = query.value(14).toInt();
        student.creditsCompleted = query.value(15).toInt();
        student.gpa = query.value(16).toDouble();
        student.mustChangePassword = query.value(17).toBool();
        return student;
    }
    
    return std::nullopt;
}

QList<Student> StudentRepo::findAll()
{
    QList<Student> students;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, student_code, name, email, phone, password_hash, salt, "
                  "program_id, department_id, national_id, birth_date, gender, seat_number, "
                  "status, level, credits_completed, gpa, must_change_password "
                  "FROM students ORDER BY student_code");
    
    if (!query.exec()) {
        qDebug() << "StudentRepo::findAll error:" << query.lastError().text();
        return students;
    }
    
    while (query.next()) {
        Student student;
        student.id = query.value(0).toInt();
        student.studentCode = query.value(1).toString();
        student.name = query.value(2).toString();
        student.email = query.value(3).toString();
        student.phone = query.value(4).toString();
        student.passwordHash = query.value(5).toString();
        student.salt = query.value(6).toString();
        student.programId = query.value(7).toInt();
        student.departmentId = query.value(8).toInt();
        student.nationalId = query.value(9).toString();
        student.birthDate = query.value(10).toDate();
        student.gender = Student::genderFromString(query.value(11).toString());
        student.seatNumber = query.value(12).toString();
        student.status = Student::statusFromString(query.value(13).toString());
        student.level = query.value(14).toInt();
        student.creditsCompleted = query.value(15).toInt();
        student.gpa = query.value(16).toDouble();
        student.mustChangePassword = query.value(17).toBool();
        students.append(student);
    }
    
    return students;
}

QList<Student> StudentRepo::findByProgram(int programId)
{
    QList<Student> students;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, student_code, name, email, phone, password_hash, salt, "
                  "program_id, department_id, national_id, birth_date, gender, seat_number, "
                  "status, level, credits_completed, gpa, must_change_password "
                  "FROM students WHERE program_id = :program_id ORDER BY student_code");
    query.bindValue(":program_id", programId);
    
    if (!query.exec()) {
        qDebug() << "StudentRepo::findByProgram error:" << query.lastError().text();
        return students;
    }
    
    while (query.next()) {
        Student student;
        student.id = query.value(0).toInt();
        student.studentCode = query.value(1).toString();
        student.name = query.value(2).toString();
        student.email = query.value(3).toString();
        student.phone = query.value(4).toString();
        student.passwordHash = query.value(5).toString();
        student.salt = query.value(6).toString();
        student.programId = query.value(7).toInt();
        student.departmentId = query.value(8).toInt();
        student.nationalId = query.value(9).toString();
        student.birthDate = query.value(10).toDate();
        student.gender = Student::genderFromString(query.value(11).toString());
        student.seatNumber = query.value(12).toString();
        student.status = Student::statusFromString(query.value(13).toString());
        student.level = query.value(14).toInt();
        student.creditsCompleted = query.value(15).toInt();
        student.gpa = query.value(16).toDouble();
        student.mustChangePassword = query.value(17).toBool();
        students.append(student);
    }
    
    return students;
}

QList<Student> StudentRepo::findByDepartment(int departmentId)
{
    QList<Student> students;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, student_code, name, email, phone, password_hash, salt, "
                  "program_id, department_id, national_id, birth_date, gender, seat_number, "
                  "status, level, credits_completed, gpa, must_change_password "
                  "FROM students WHERE department_id = :department_id ORDER BY student_code");
    query.bindValue(":department_id", departmentId);
    
    if (!query.exec()) {
        qDebug() << "StudentRepo::findByDepartment error:" << query.lastError().text();
        return students;
    }
    
    while (query.next()) {
        Student student;
        student.id = query.value(0).toInt();
        student.studentCode = query.value(1).toString();
        student.name = query.value(2).toString();
        student.email = query.value(3).toString();
        student.phone = query.value(4).toString();
        student.passwordHash = query.value(5).toString();
        student.salt = query.value(6).toString();
        student.programId = query.value(7).toInt();
        student.departmentId = query.value(8).toInt();
        student.nationalId = query.value(9).toString();
        student.birthDate = query.value(10).toDate();
        student.gender = Student::genderFromString(query.value(11).toString());
        student.seatNumber = query.value(12).toString();
        student.status = Student::statusFromString(query.value(13).toString());
        student.level = query.value(14).toInt();
        student.creditsCompleted = query.value(15).toInt();
        student.gpa = query.value(16).toDouble();
        student.mustChangePassword = query.value(17).toBool();
        students.append(student);
    }
    
    return students;
}

bool StudentRepo::save(const Student& student)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO students (student_code, name, email, phone, password_hash, salt, "
                  "program_id, department_id, national_id, birth_date, gender, seat_number, "
                  "status, level, credits_completed, gpa, must_change_password) "
                  "VALUES (:code, :name, :email, :phone, :hash, :salt, :program, :dept, "
                  ":nid, :birth, :gender, :seat, :status, :level, :credits, :gpa, :must_change)");
    
    query.bindValue(":code", student.studentCode);
    query.bindValue(":name", student.name);
    query.bindValue(":email", student.email);
    query.bindValue(":phone", student.phone);
    query.bindValue(":hash", student.passwordHash);
    query.bindValue(":salt", student.salt);
    query.bindValue(":program", student.programId);
    query.bindValue(":dept", student.departmentId);
    query.bindValue(":nid", student.nationalId);
    query.bindValue(":birth", student.birthDate);
    query.bindValue(":gender", student.genderString());
    query.bindValue(":seat", student.seatNumber);
    query.bindValue(":status", student.statusString());
    query.bindValue(":level", student.level);
    query.bindValue(":credits", student.creditsCompleted);
    query.bindValue(":gpa", student.gpa);
    query.bindValue(":must_change", student.mustChangePassword);
    
    if (!query.exec()) {
        qDebug() << "StudentRepo::save error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool StudentRepo::update(const Student& student)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE students SET name = :name, email = :email, phone = :phone, "
                  "program_id = :program, department_id = :dept, national_id = :nid, "
                  "birth_date = :birth, gender = :gender, seat_number = :seat, "
                  "status = :status, level = :level, credits_completed = :credits, "
                  "gpa = :gpa, must_change_password = :must_change "
                  "WHERE id = :id");
    
    query.bindValue(":id", student.id);
    query.bindValue(":name", student.name);
    query.bindValue(":email", student.email);
    query.bindValue(":phone", student.phone);
    query.bindValue(":program", student.programId);
    query.bindValue(":dept", student.departmentId);
    query.bindValue(":nid", student.nationalId);
    query.bindValue(":birth", student.birthDate);
    query.bindValue(":gender", student.genderString());
    query.bindValue(":seat", student.seatNumber);
    query.bindValue(":status", student.statusString());
    query.bindValue(":level", student.level);
    query.bindValue(":credits", student.creditsCompleted);
    query.bindValue(":gpa", student.gpa);
    query.bindValue(":must_change", student.mustChangePassword);
    
    if (!query.exec()) {
        qDebug() << "StudentRepo::update error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool StudentRepo::remove(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM students WHERE id = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qDebug() << "StudentRepo::remove error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool StudentRepo::updatePassword(int id, const QString& newPasswordHash, const QString& newSalt)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE students SET password_hash = :hash, salt = :salt, must_change_password = 0 WHERE id = :id");
    query.bindValue(":hash", newPasswordHash);
    query.bindValue(":salt", newSalt);
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qDebug() << "StudentRepo::updatePassword error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool StudentRepo::updateGPA(int id, double gpa, int creditsCompleted)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE students SET gpa = :gpa, credits_completed = :credits WHERE id = :id");
    query.bindValue(":gpa", gpa);
    query.bindValue(":credits", creditsCompleted);
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qDebug() << "StudentRepo::updateGPA error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

