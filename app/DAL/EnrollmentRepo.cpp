#include "EnrollmentRepo.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <optional>

EnrollmentRepo::EnrollmentRepo()
{
}

std::optional<Enrollment> EnrollmentRepo::findById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, student_id, course_id, semester, year, status, attempt_number, registered_at "
                  "FROM enrollments WHERE id = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qDebug() << "EnrollmentRepo::findById error:" << query.lastError().text();
        return std::nullopt;
    }
    
    if (query.next()) {
        Enrollment enrollment;
        enrollment.id = query.value(0).toInt();
        enrollment.studentId = query.value(1).toInt();
        enrollment.courseId = query.value(2).toInt();
        enrollment.semester = query.value(3).toInt();
        enrollment.year = query.value(4).toInt();
        enrollment.status = Enrollment::statusFromString(query.value(5).toString());
        enrollment.attemptNumber = query.value(6).toInt();
        enrollment.registeredAt = query.value(7).toDateTime();
        return enrollment;
    }
    
    return std::nullopt;
}

QList<Enrollment> EnrollmentRepo::findByStudent(int studentId)
{
    QList<Enrollment> enrollments;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, student_id, course_id, semester, year, status, attempt_number, registered_at "
                  "FROM enrollments WHERE student_id = :student_id ORDER BY year DESC, semester DESC");
    query.bindValue(":student_id", studentId);
    
    if (!query.exec()) {
        qDebug() << "EnrollmentRepo::findByStudent error:" << query.lastError().text();
        return enrollments;
    }
    
    while (query.next()) {
        Enrollment enrollment;
        enrollment.id = query.value(0).toInt();
        enrollment.studentId = query.value(1).toInt();
        enrollment.courseId = query.value(2).toInt();
        enrollment.semester = query.value(3).toInt();
        enrollment.year = query.value(4).toInt();
        enrollment.status = Enrollment::statusFromString(query.value(5).toString());
        enrollment.attemptNumber = query.value(6).toInt();
        enrollment.registeredAt = query.value(7).toDateTime();
        enrollments.append(enrollment);
    }
    
    return enrollments;
}

QList<Enrollment> EnrollmentRepo::findByCourse(int courseId)
{
    QList<Enrollment> enrollments;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, student_id, course_id, semester, year, status, attempt_number, registered_at "
                  "FROM enrollments WHERE course_id = :course_id AND status = 'registered' ORDER BY student_id");
    query.bindValue(":course_id", courseId);
    
    if (!query.exec()) {
        qDebug() << "EnrollmentRepo::findByCourse error:" << query.lastError().text();
        return enrollments;
    }
    
    while (query.next()) {
        Enrollment enrollment;
        enrollment.id = query.value(0).toInt();
        enrollment.studentId = query.value(1).toInt();
        enrollment.courseId = query.value(2).toInt();
        enrollment.semester = query.value(3).toInt();
        enrollment.year = query.value(4).toInt();
        enrollment.status = Enrollment::statusFromString(query.value(5).toString());
        enrollment.attemptNumber = query.value(6).toInt();
        enrollment.registeredAt = query.value(7).toDateTime();
        enrollments.append(enrollment);
    }
    
    return enrollments;
}

QList<Enrollment> EnrollmentRepo::findByStudentAndSemester(int studentId, int semester, int year)
{
    QList<Enrollment> enrollments;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, student_id, course_id, semester, year, status, attempt_number, registered_at "
                  "FROM enrollments WHERE student_id = :student_id AND semester = :semester AND year = :year");
    query.bindValue(":student_id", studentId);
    query.bindValue(":semester", semester);
    query.bindValue(":year", year);
    
    if (!query.exec()) {
        qDebug() << "EnrollmentRepo::findByStudentAndSemester error:" << query.lastError().text();
        return enrollments;
    }
    
    while (query.next()) {
        Enrollment enrollment;
        enrollment.id = query.value(0).toInt();
        enrollment.studentId = query.value(1).toInt();
        enrollment.courseId = query.value(2).toInt();
        enrollment.semester = query.value(3).toInt();
        enrollment.year = query.value(4).toInt();
        enrollment.status = Enrollment::statusFromString(query.value(5).toString());
        enrollment.attemptNumber = query.value(6).toInt();
        enrollment.registeredAt = query.value(7).toDateTime();
        enrollments.append(enrollment);
    }
    
    return enrollments;
}

bool EnrollmentRepo::save(const Enrollment& enrollment)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO enrollments (student_id, course_id, semester, year, status, attempt_number) "
                  "VALUES (:student, :course, :semester, :year, :status, :attempt)");
    
    query.bindValue(":student", enrollment.studentId);
    query.bindValue(":course", enrollment.courseId);
    query.bindValue(":semester", enrollment.semester);
    query.bindValue(":year", enrollment.year);
    query.bindValue(":status", enrollment.statusString());
    query.bindValue(":attempt", enrollment.attemptNumber);
    
    if (!query.exec()) {
        qDebug() << "EnrollmentRepo::save error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool EnrollmentRepo::update(const Enrollment& enrollment)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE enrollments SET student_id = :student, course_id = :course, "
                  "semester = :semester, year = :year, status = :status, attempt_number = :attempt "
                  "WHERE id = :id");
    
    query.bindValue(":id", enrollment.id);
    query.bindValue(":student", enrollment.studentId);
    query.bindValue(":course", enrollment.courseId);
    query.bindValue(":semester", enrollment.semester);
    query.bindValue(":year", enrollment.year);
    query.bindValue(":status", enrollment.statusString());
    query.bindValue(":attempt", enrollment.attemptNumber);
    
    if (!query.exec()) {
        qDebug() << "EnrollmentRepo::update error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool EnrollmentRepo::remove(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM enrollments WHERE id = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qDebug() << "EnrollmentRepo::remove error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool EnrollmentRepo::approveEnrollment(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE enrollments SET status = 'registered' WHERE id = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qDebug() << "EnrollmentRepo::approveEnrollment error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool EnrollmentRepo::rejectEnrollment(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE enrollments SET status = 'dropped' WHERE id = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qDebug() << "EnrollmentRepo::rejectEnrollment error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

