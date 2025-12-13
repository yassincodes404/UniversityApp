#include "PMDService.h"
#include "../DAL/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

bool PMDService::createPMDRecord(int studentId, int courseId, int examId, 
                                  const QString& status, const QString& notes)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO pmd_records (student_id, course_id, exam_id, pmd_status, notes, created_at) "
        "VALUES (?, ?, ?, ?, ?, ?)"
    );
    
    query.addBindValue(studentId);
    query.addBindValue(courseId);
    query.addBindValue(examId);
    query.addBindValue(status);
    query.addBindValue(notes);
    query.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    
    if (!query.exec()) {
        qDebug() << "PMDService::createPMDRecord error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool PMDService::hasPMD(int studentId, int courseId)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT COUNT(*) FROM pmd_records WHERE student_id = ? AND course_id = ?");
    query.addBindValue(studentId);
    query.addBindValue(courseId);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    
    return false;
}

