#include "ExamRepo.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

ExamRepo::ExamRepo()
{
}

QList<Exam> ExamRepo::findByCourse(int courseId)
{
    QList<Exam> exams;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM exams WHERE course_id = ? ORDER BY exam_date");
    query.addBindValue(courseId);
    
    if (query.exec()) {
        while (query.next()) {
            Exam exam;
            exam.setId(query.value("id").toInt());
            exam.setCourseId(query.value("course_id").toInt());
            exam.setExamType(query.value("exam_type").toString());
            exam.setExamDate(QDate::fromString(query.value("exam_date").toString(), "yyyy-MM-dd"));
            exam.setYear(query.value("year").toInt());
            exam.setSemester(query.value("semester").toInt());
            exam.setDurationMinutes(query.value("duration_minutes").toInt());
            exam.setVenue(query.value("venue").toString());
            exam.setCreatedAt(QDateTime::fromString(query.value("created_at").toString(), "yyyy-MM-dd HH:mm:ss"));
            exams.append(exam);
        }
    }
    
    return exams;
}

bool ExamRepo::save(Exam& exam)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO exams (course_id, exam_type, exam_date, year, semester, duration_minutes, venue) "
        "VALUES (?, ?, ?, ?, ?, ?, ?)"
    );
    
    query.addBindValue(exam.courseId());
    query.addBindValue(exam.examType());
    query.addBindValue(exam.examDate().toString("yyyy-MM-dd"));
    query.addBindValue(exam.year());
    query.addBindValue(exam.semester());
    query.addBindValue(exam.durationMinutes());
    query.addBindValue(exam.venue());
    
    if (!query.exec()) {
        qDebug() << "ExamRepo::save error:" << query.lastError().text();
        return false;
    }
    
    exam.setId(query.lastInsertId().toInt());
    return true;
}

