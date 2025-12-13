#include "GradeRepo.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

GradeRepo::GradeRepo()
{
}

std::optional<Grade> GradeRepo::findById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM grades WHERE id = ?");
    query.addBindValue(id);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    return mapFromQuery(query);
}

std::optional<Grade> GradeRepo::findByEnrollment(int enrollmentId)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM grades WHERE enrollment_id = ?");
    query.addBindValue(enrollmentId);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    return mapFromQuery(query);
}

std::optional<Grade> GradeRepo::findByStudentAndCourse(int studentId, int courseId)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM grades WHERE student_id = ? AND course_id = ?");
    query.addBindValue(studentId);
    query.addBindValue(courseId);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    return mapFromQuery(query);
}

QList<Grade> GradeRepo::findByStudent(int studentId)
{
    QList<Grade> grades;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM grades WHERE student_id = ? ORDER BY recorded_at DESC");
    query.addBindValue(studentId);
    
    if (query.exec()) {
        while (query.next()) {
            grades.append(mapFromQuery(query));
        }
    }
    
    return grades;
}

QList<Grade> GradeRepo::findByCourse(int courseId)
{
    QList<Grade> grades;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM grades WHERE course_id = ? ORDER BY student_id");
    query.addBindValue(courseId);
    
    if (query.exec()) {
        while (query.next()) {
            grades.append(mapFromQuery(query));
        }
    }
    
    return grades;
}

bool GradeRepo::save(Grade& grade)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO grades (enrollment_id, student_id, course_id, midterm1, midterm2, "
        "final_exam, practical, assignments, total, letter_grade, result_status, is_published, recorded_at) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
    );
    
    query.addBindValue(grade.enrollmentId());
    query.addBindValue(grade.studentId());
    query.addBindValue(grade.courseId());
    query.addBindValue(grade.midterm1());
    query.addBindValue(grade.midterm2());
    query.addBindValue(grade.finalExam());
    query.addBindValue(grade.practical());
    query.addBindValue(grade.assignments());
    query.addBindValue(grade.total());
    query.addBindValue(grade.letterGrade());
    query.addBindValue(grade.resultStatus());
    query.addBindValue(grade.isPublished() ? 1 : 0);
    query.addBindValue(grade.recordedAt().toString("yyyy-MM-dd HH:mm:ss"));
    
    if (!query.exec()) {
        qDebug() << "GradeRepo::save error:" << query.lastError().text();
        return false;
    }
    
    grade.setId(query.lastInsertId().toInt());
    return true;
}

bool GradeRepo::update(const Grade& grade)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "UPDATE grades SET enrollment_id = ?, student_id = ?, course_id = ?, "
        "midterm1 = ?, midterm2 = ?, final_exam = ?, practical = ?, assignments = ?, "
        "total = ?, letter_grade = ?, result_status = ?, is_published = ? WHERE id = ?"
    );
    
    query.addBindValue(grade.enrollmentId());
    query.addBindValue(grade.studentId());
    query.addBindValue(grade.courseId());
    query.addBindValue(grade.midterm1());
    query.addBindValue(grade.midterm2());
    query.addBindValue(grade.finalExam());
    query.addBindValue(grade.practical());
    query.addBindValue(grade.assignments());
    query.addBindValue(grade.total());
    query.addBindValue(grade.letterGrade());
    query.addBindValue(grade.resultStatus());
    query.addBindValue(grade.isPublished() ? 1 : 0);
    query.addBindValue(grade.id());
    
    if (!query.exec()) {
        qDebug() << "GradeRepo::update error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool GradeRepo::remove(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM grades WHERE id = ?");
    query.addBindValue(id);
    
    return query.exec();
}

bool GradeRepo::publishGrades(int courseId)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE grades SET is_published = 1 WHERE course_id = ?");
    query.addBindValue(courseId);
    
    return query.exec();
}

Grade GradeRepo::mapFromQuery(const QSqlQuery& query)
{
    Grade grade;
    grade.setId(query.value("id").toInt());
    grade.setEnrollmentId(query.value("enrollment_id").toInt());
    grade.setStudentId(query.value("student_id").toInt());
    grade.setCourseId(query.value("course_id").toInt());
    grade.setMidterm1(query.value("midterm1").toDouble());
    grade.setMidterm2(query.value("midterm2").toDouble());
    grade.setFinalExam(query.value("final_exam").toDouble());
    grade.setPractical(query.value("practical").toDouble());
    grade.setAssignments(query.value("assignments").toDouble());
    grade.setTotal(query.value("total").toDouble());
    grade.setLetterGrade(query.value("letter_grade").toString());
    grade.setResultStatus(query.value("result_status").toString());
    grade.setIsPublished(query.value("is_published").toInt() == 1);
    grade.setRecordedAt(QDateTime::fromString(query.value("recorded_at").toString(), "yyyy-MM-dd HH:mm:ss"));
    
    return grade;
}

