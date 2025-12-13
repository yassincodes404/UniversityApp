#include "StudentRepo.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>

StudentRepo::StudentRepo()
{
}

std::optional<Student> StudentRepo::findById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM students WHERE id = ?");
    query.addBindValue(id);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    return mapFromQuery(query);
}

std::optional<Student> StudentRepo::findByCode(const QString& code)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM students WHERE student_code = ?");
    query.addBindValue(code);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    return mapFromQuery(query);
}

std::optional<Student> StudentRepo::findByEmail(const QString& email)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM students WHERE email = ?");
    query.addBindValue(email);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    return mapFromQuery(query);
}

QList<Student> StudentRepo::findAll()
{
    QList<Student> students;
    QSqlQuery query("SELECT * FROM students ORDER BY student_code", DatabaseManager::instance().database());
    
    while (query.next()) {
        students.append(mapFromQuery(query));
    }
    
    return students;
}

QList<Student> StudentRepo::findByProgram(int programId)
{
    QList<Student> students;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM students WHERE program_id = ? ORDER BY student_code");
    query.addBindValue(programId);
    
    if (query.exec()) {
        while (query.next()) {
            students.append(mapFromQuery(query));
        }
    }
    
    return students;
}

QList<Student> StudentRepo::findByDepartment(int departmentId)
{
    QList<Student> students;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM students WHERE department_id = ? ORDER BY student_code");
    query.addBindValue(departmentId);
    
    if (query.exec()) {
        while (query.next()) {
            students.append(mapFromQuery(query));
        }
    }
    
    return students;
}

bool StudentRepo::save(Student& student)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO students (student_code, name, email, phone, password_hash, salt, "
        "program_id, department_id, national_id, birth_date, gender, seat_number, "
        "status, level, credits_completed, gpa, must_change_password) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
    );
    
    query.addBindValue(student.studentCode());
    query.addBindValue(student.name());
    query.addBindValue(student.email());
    query.addBindValue(student.phone());
    query.addBindValue(student.passwordHash());
    query.addBindValue(student.salt());
    query.addBindValue(student.programId());
    query.addBindValue(student.departmentId());
    query.addBindValue(student.nationalId());
    query.addBindValue(student.birthDate().toString("yyyy-MM-dd"));
    query.addBindValue(student.gender());
    query.addBindValue(student.seatNumber());
    query.addBindValue(student.status());
    query.addBindValue(student.level());
    query.addBindValue(student.creditsCompleted());
    query.addBindValue(student.gpa());
    query.addBindValue(student.mustChangePassword() ? 1 : 0);
    
    if (!query.exec()) {
        qDebug() << "StudentRepo::save error:" << query.lastError().text();
        return false;
    }
    
    student.setId(query.lastInsertId().toInt());
    return true;
}

bool StudentRepo::update(const Student& student)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "UPDATE students SET student_code = ?, name = ?, email = ?, phone = ?, "
        "password_hash = ?, salt = ?, program_id = ?, department_id = ?, "
        "national_id = ?, birth_date = ?, gender = ?, seat_number = ?, "
        "status = ?, level = ?, credits_completed = ?, gpa = ?, must_change_password = ? "
        "WHERE id = ?"
    );
    
    query.addBindValue(student.studentCode());
    query.addBindValue(student.name());
    query.addBindValue(student.email());
    query.addBindValue(student.phone());
    query.addBindValue(student.passwordHash());
    query.addBindValue(student.salt());
    query.addBindValue(student.programId());
    query.addBindValue(student.departmentId());
    query.addBindValue(student.nationalId());
    query.addBindValue(student.birthDate().toString("yyyy-MM-dd"));
    query.addBindValue(student.gender());
    query.addBindValue(student.seatNumber());
    query.addBindValue(student.status());
    query.addBindValue(student.level());
    query.addBindValue(student.creditsCompleted());
    query.addBindValue(student.gpa());
    query.addBindValue(student.mustChangePassword() ? 1 : 0);
    query.addBindValue(student.id());
    
    if (!query.exec()) {
        qDebug() << "StudentRepo::update error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool StudentRepo::remove(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM students WHERE id = ?");
    query.addBindValue(id);
    
    return query.exec();
}

bool StudentRepo::exists(const QString& studentCode)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT COUNT(*) FROM students WHERE student_code = ?");
    query.addBindValue(studentCode);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    
    return false;
}

Student StudentRepo::mapFromQuery(const QSqlQuery& query)
{
    Student student;
    student.setId(query.value("id").toInt());
    student.setStudentCode(query.value("student_code").toString());
    student.setName(query.value("name").toString());
    student.setEmail(query.value("email").toString());
    student.setPhone(query.value("phone").toString());
    student.setPasswordHash(query.value("password_hash").toString());
    student.setSalt(query.value("salt").toString());
    student.setProgramId(query.value("program_id").toInt());
    student.setDepartmentId(query.value("department_id").toInt());
    student.setNationalId(query.value("national_id").toString());
    student.setBirthDate(QDate::fromString(query.value("birth_date").toString(), "yyyy-MM-dd"));
    student.setGender(query.value("gender").toString());
    student.setSeatNumber(query.value("seat_number").toString());
    student.setStatus(query.value("status").toString());
    student.setLevel(query.value("level").toInt());
    student.setCreditsCompleted(query.value("credits_completed").toInt());
    student.setGpa(query.value("gpa").toDouble());
    student.setMustChangePassword(query.value("must_change_password").toInt() == 1);
    
    return student;
}

