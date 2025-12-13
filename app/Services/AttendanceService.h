#ifndef ATTENDANCESERVICE_H
#define ATTENDANCESERVICE_H

#include <QDate>
#include <QString>

struct AttendanceRecord {
    int studentId;
    int sectionId;
    QDate date;
    QString status; // present, absent, late, excused
    QString remark;
};

class AttendanceService
{
public:
    AttendanceService();
    
    // Record attendance
    bool recordAttendance(const AttendanceRecord& record);
    
    // Calculate attendance percentage for a student in a section
    double calculateAttendancePercentage(int studentId, int sectionId);
    
    // Check if student is barred (exceeded 25% absence)
    bool isBarred(int studentId, int sectionId);
    
    // Get attendance records for a student in a section
    QList<AttendanceRecord> getAttendanceRecords(int studentId, int sectionId);
};

#endif // ATTENDANCESERVICE_H

