#ifndef ATTENDANCEREPO_H
#define ATTENDANCEREPO_H

#include "../Models/Attendance.h"
#include <QList>
#include <optional>
#include <QDate>

class QSqlQuery;

class AttendanceRepo
{
public:
    AttendanceRepo();
    
    std::optional<Attendance> findById(int id);
    QList<Attendance> findByStudentAndSection(int studentId, int sectionId);
    QList<Attendance> findBySection(int sectionId);
    QList<Attendance> findBySectionAndDate(int sectionId, const QDate& date);
    
    bool save(Attendance& attendance);
    bool update(const Attendance& attendance);
    bool remove(int id);
    bool markAttendance(int studentId, int sectionId, const QDate& date, const QString& status);
    
    int getPresentCount(int studentId, int sectionId);
    int getTotalCount(int studentId, int sectionId);
    double getAttendancePercentage(int studentId, int sectionId);

private:
    Attendance mapFromQuery(const QSqlQuery& query);
};

#endif // ATTENDANCEREPO_H

