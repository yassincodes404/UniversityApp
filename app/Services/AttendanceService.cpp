#include "AttendanceService.h"
#include "../DAL/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AttendanceService::AttendanceService()
{
}

bool AttendanceService::recordAttendance(const AttendanceRecord& record)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO attendance (student_id, section_id, attendance_date, status, remark) "
                  "VALUES (:student, :section, :date, :status, :remark) "
                  "ON DUPLICATE KEY UPDATE status = :status, remark = :remark");
    
    query.bindValue(":student", record.studentId);
    query.bindValue(":section", record.sectionId);
    query.bindValue(":date", record.date);
    query.bindValue(":status", record.status);
    query.bindValue(":remark", record.remark);
    
    if (!query.exec()) {
        qDebug() << "AttendanceService::recordAttendance error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

double AttendanceService::calculateAttendancePercentage(int studentId, int sectionId)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT COUNT(*) as total, SUM(CASE WHEN status = 'present' THEN 1 ELSE 0 END) as present "
                  "FROM attendance WHERE student_id = :student AND section_id = :section");
    query.bindValue(":student", studentId);
    query.bindValue(":section", sectionId);
    
    if (!query.exec() || !query.next()) {
        return 0.0;
    }
    
    int total = query.value(0).toInt();
    int present = query.value(1).toInt();
    
    if (total == 0) return 100.0;
    
    return (static_cast<double>(present) / total) * 100.0;
}

bool AttendanceService::isBarred(int studentId, int sectionId)
{
    double percentage = calculateAttendancePercentage(studentId, sectionId);
    return percentage < 75.0; // Less than 75% means more than 25% absence
}

QList<AttendanceRecord> AttendanceService::getAttendanceRecords(int studentId, int sectionId)
{
    QList<AttendanceRecord> records;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT student_id, section_id, attendance_date, status, remark "
                  "FROM attendance WHERE student_id = :student AND section_id = :section "
                  "ORDER BY attendance_date DESC");
    query.bindValue(":student", studentId);
    query.bindValue(":section", sectionId);
    
    if (!query.exec()) {
        qDebug() << "AttendanceService::getAttendanceRecords error:" << query.lastError().text();
        return records;
    }
    
    while (query.next()) {
        AttendanceRecord record;
        record.studentId = query.value(0).toInt();
        record.sectionId = query.value(1).toInt();
        record.date = query.value(2).toDate();
        record.status = query.value(3).toString();
        record.remark = query.value(4).toString();
        records.append(record);
    }
    
    return records;
}

