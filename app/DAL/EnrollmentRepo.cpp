#include "EnrollmentRepo.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

EnrollmentRepo::EnrollmentRepo()
{
}

std::optional<Enrollment> EnrollmentRepo::findById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM enrollments WHERE id = ?");
    query.addBindValue(id);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    return mapFromQuery(query);
}

QList<Enrollment> EnrollmentRepo::findByStudent(int studentId)
{
    QList<Enrollment> enrollments;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM enrollments WHERE student_id = ? ORDER BY year DESC, semester DESC");
    query.addBindValue(studentId);
    
    if (query.exec()) {
        while (query.next()) {
            enrollments.append(mapFromQuery(query));
        }
    }
    
    return enrollments;
}

QList<Enrollment> EnrollmentRepo::findByStudentAndSemester(int studentId, int semester, int year)
{
    QList<Enrollment> enrollments;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM enrollments WHERE student_id = ? AND semester = ? AND year = ?");
    query.addBindValue(studentId);
    query.addBindValue(semester);
    query.addBindValue(year);
    
    if (query.exec()) {
        while (query.next()) {
            enrollments.append(mapFromQuery(query));
        }
    }
    
    return enrollments;
}

QList<Enrollment> EnrollmentRepo::findByCourse(int courseId)
{
    QList<Enrollment> enrollments;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM enrollments WHERE course_id = ?");
    query.addBindValue(courseId);
    
    if (query.exec()) {
        while (query.next()) {
            enrollments.append(mapFromQuery(query));
        }
    }
    
    return enrollments;
}

QList<Enrollment> EnrollmentRepo::findPending()
{
    QList<Enrollment> enrollments;
    QSqlQuery query("SELECT * FROM enrollments WHERE status = 'pending' ORDER BY registered_at", DatabaseManager::instance().database());
    
    while (query.next()) {
        enrollments.append(mapFromQuery(query));
    }
    
    return enrollments;
}

bool EnrollmentRepo::save(Enrollment& enrollment)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO enrollments (student_id, course_id, semester, year, status, attempt_number, registered_at) "
        "VALUES (?, ?, ?, ?, ?, ?, ?)"
    );
    
    query.addBindValue(enrollment.studentId());
    query.addBindValue(enrollment.courseId());
    query.addBindValue(enrollment.semester());
    query.addBindValue(enrollment.year());
    query.addBindValue(enrollment.status());
    query.addBindValue(enrollment.attemptNumber());
    query.addBindValue(enrollment.registeredAt().toString("yyyy-MM-dd HH:mm:ss"));
    
    if (!query.exec()) {
        qDebug() << "EnrollmentRepo::save error:" << query.lastError().text();
        return false;
    }
    
    enrollment.setId(query.lastInsertId().toInt());
    return true;
}

bool EnrollmentRepo::update(const Enrollment& enrollment)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "UPDATE enrollments SET student_id = ?, course_id = ?, semester = ?, year = ?, "
        "status = ?, attempt_number = ? WHERE id = ?"
    );
    
    query.addBindValue(enrollment.studentId());
    query.addBindValue(enrollment.courseId());
    query.addBindValue(enrollment.semester());
    query.addBindValue(enrollment.year());
    query.addBindValue(enrollment.status());
    query.addBindValue(enrollment.attemptNumber());
    query.addBindValue(enrollment.id());
    
    if (!query.exec()) {
        qDebug() << "EnrollmentRepo::update error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool EnrollmentRepo::remove(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM enrollments WHERE id = ?");
    query.addBindValue(id);
    
    return query.exec();
}

bool EnrollmentRepo::approve(int enrollmentId)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE enrollments SET status = 'registered' WHERE id = ?");
    query.addBindValue(enrollmentId);
    
    return query.exec();
}

bool EnrollmentRepo::reject(int enrollmentId)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM enrollments WHERE id = ?");
    query.addBindValue(enrollmentId);
    
    return query.exec();
}

Enrollment EnrollmentRepo::mapFromQuery(const QSqlQuery& query)
{
    Enrollment enrollment;
    enrollment.setId(query.value("id").toInt());
    enrollment.setStudentId(query.value("student_id").toInt());
    enrollment.setCourseId(query.value("course_id").toInt());
    enrollment.setSemester(query.value("semester").toInt());
    enrollment.setYear(query.value("year").toInt());
    enrollment.setStatus(query.value("status").toString());
    enrollment.setAttemptNumber(query.value("attempt_number").toInt());
    enrollment.setRegisteredAt(QDateTime::fromString(query.value("registered_at").toString(), "yyyy-MM-dd HH:mm:ss"));
    
    return enrollment;
}

