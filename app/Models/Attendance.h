#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <QString>
#include <QDate>
#include <QDateTime>

class Attendance
{
public:
    Attendance();
    
    int id() const { return m_id; }
    void setId(int id) { m_id = id; }
    
    int studentId() const { return m_studentId; }
    void setStudentId(int id) { m_studentId = id; }
    
    int sectionId() const { return m_sectionId; }
    void setSectionId(int id) { m_sectionId = id; }
    
    QDate attendanceDate() const { return m_attendanceDate; }
    void setAttendanceDate(const QDate& date) { m_attendanceDate = date; }
    
    QString status() const { return m_status; }
    void setStatus(const QString& status) { m_status = status; }
    
    QString remark() const { return m_remark; }
    void setRemark(const QString& remark) { m_remark = remark; }
    
    QDateTime recordedAt() const { return m_recordedAt; }
    void setRecordedAt(const QDateTime& dt) { m_recordedAt = dt; }

private:
    int m_id = 0;
    int m_studentId = 0;
    int m_sectionId = 0;
    QDate m_attendanceDate;
    QString m_status;
    QString m_remark;
    QDateTime m_recordedAt;
};

#endif // ATTENDANCE_H

