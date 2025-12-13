#include "AttendanceRepo.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

AttendanceRepo::AttendanceRepo()
{
}

std::optional<Attendance> AttendanceRepo::findById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM attendance WHERE id = ?");
    query.addBindValue(id);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    return mapFromQuery(query);
}

QList<Attendance> AttendanceRepo::findByStudentAndSection(int studentId, int sectionId)
{
    QList<Attendance> attendances;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM attendance WHERE student_id = ? AND section_id = ? ORDER BY attendance_date");
    query.addBindValue(studentId);
    query.addBindValue(sectionId);
    
    if (query.exec()) {
        while (query.next()) {
            attendances.append(mapFromQuery(query));
        }
    }
    
    return attendances;
}

QList<Attendance> AttendanceRepo::findBySection(int sectionId)
{
    QList<Attendance> attendances;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM attendance WHERE section_id = ? ORDER BY attendance_date, student_id");
    query.addBindValue(sectionId);
    
    if (query.exec()) {
        while (query.next()) {
            attendances.append(mapFromQuery(query));
        }
    }
    
    return attendances;
}

QList<Attendance> AttendanceRepo::findBySectionAndDate(int sectionId, const QDate& date)
{
    QList<Attendance> attendances;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM attendance WHERE section_id = ? AND attendance_date = ?");
    query.addBindValue(sectionId);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    
    if (query.exec()) {
        while (query.next()) {
            attendances.append(mapFromQuery(query));
        }
    }
    
    return attendances;
}

bool AttendanceRepo::save(Attendance& attendance)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO attendance (student_id, section_id, attendance_date, status, remark, recorded_at) "
        "VALUES (?, ?, ?, ?, ?, ?)"
    );
    
    query.addBindValue(attendance.studentId());
    query.addBindValue(attendance.sectionId());
    query.addBindValue(attendance.attendanceDate().toString("yyyy-MM-dd"));
    query.addBindValue(attendance.status());
    query.addBindValue(attendance.remark());
    query.addBindValue(attendance.recordedAt().toString("yyyy-MM-dd HH:mm:ss"));
    
    if (!query.exec()) {
        qDebug() << "AttendanceRepo::save error:" << query.lastError().text();
        return false;
    }
    
    attendance.setId(query.lastInsertId().toInt());
    return true;
}

bool AttendanceRepo::update(const Attendance& attendance)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "UPDATE attendance SET student_id = ?, section_id = ?, attendance_date = ?, "
        "status = ?, remark = ? WHERE id = ?"
    );
    
    query.addBindValue(attendance.studentId());
    query.addBindValue(attendance.sectionId());
    query.addBindValue(attendance.attendanceDate().toString("yyyy-MM-dd"));
    query.addBindValue(attendance.status());
    query.addBindValue(attendance.remark());
    query.addBindValue(attendance.id());
    
    if (!query.exec()) {
        qDebug() << "AttendanceRepo::update error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool AttendanceRepo::remove(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM attendance WHERE id = ?");
    query.addBindValue(id);
    
    return query.exec();
}

bool AttendanceRepo::markAttendance(int studentId, int sectionId, const QDate& date, const QString& status)
{
    // Check if already exists
    QSqlQuery checkQuery(DatabaseManager::instance().database());
    checkQuery.prepare("SELECT id FROM attendance WHERE student_id = ? AND section_id = ? AND attendance_date = ?");
    checkQuery.addBindValue(studentId);
    checkQuery.addBindValue(sectionId);
    checkQuery.addBindValue(date.toString("yyyy-MM-dd"));
    
    if (checkQuery.exec() && checkQuery.next()) {
        // Update existing
        QSqlQuery updateQuery(DatabaseManager::instance().database());
        updateQuery.prepare("UPDATE attendance SET status = ? WHERE id = ?");
        updateQuery.addBindValue(status);
        updateQuery.addBindValue(checkQuery.value(0).toInt());
        return updateQuery.exec();
    } else {
        // Insert new
        Attendance attendance;
        attendance.setStudentId(studentId);
        attendance.setSectionId(sectionId);
        attendance.setAttendanceDate(date);
        attendance.setStatus(status);
        attendance.setRecordedAt(QDateTime::currentDateTime());
        return save(attendance);
    }
}

int AttendanceRepo::getPresentCount(int studentId, int sectionId)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT COUNT(*) FROM attendance "
        "WHERE student_id = ? AND section_id = ? AND status IN ('present', 'late')"
    );
    query.addBindValue(studentId);
    query.addBindValue(sectionId);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    
    return 0;
}

int AttendanceRepo::getTotalCount(int studentId, int sectionId)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT COUNT(*) FROM attendance "
        "WHERE student_id = ? AND section_id = ?"
    );
    query.addBindValue(studentId);
    query.addBindValue(sectionId);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    
    return 0;
}

double AttendanceRepo::getAttendancePercentage(int studentId, int sectionId)
{
    int total = getTotalCount(studentId, sectionId);
    if (total == 0) return 0.0;
    
    int present = getPresentCount(studentId, sectionId);
    return (static_cast<double>(present) / total) * 100.0;
}

Attendance AttendanceRepo::mapFromQuery(const QSqlQuery& query)
{
    Attendance attendance;
    attendance.setId(query.value("id").toInt());
    attendance.setStudentId(query.value("student_id").toInt());
    attendance.setSectionId(query.value("section_id").toInt());
    attendance.setAttendanceDate(QDate::fromString(query.value("attendance_date").toString(), "yyyy-MM-dd"));
    attendance.setStatus(query.value("status").toString());
    attendance.setRemark(query.value("remark").toString());
    attendance.setRecordedAt(QDateTime::fromString(query.value("recorded_at").toString(), "yyyy-MM-dd HH:mm:ss"));
    
    return attendance;
}

