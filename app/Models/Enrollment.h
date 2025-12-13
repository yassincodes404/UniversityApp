#ifndef ENROLLMENT_H
#define ENROLLMENT_H

#include <QString>
#include <QDateTime>

class Enrollment
{
public:
    Enrollment();
    
    int id() const { return m_id; }
    void setId(int id) { m_id = id; }
    
    int studentId() const { return m_studentId; }
    void setStudentId(int id) { m_studentId = id; }
    
    int courseId() const { return m_courseId; }
    void setCourseId(int id) { m_courseId = id; }
    
    int semester() const { return m_semester; }
    void setSemester(int sem) { m_semester = sem; }
    
    int year() const { return m_year; }
    void setYear(int year) { m_year = year; }
    
    QString status() const { return m_status; }
    void setStatus(const QString& status) { m_status = status; }
    
    int attemptNumber() const { return m_attemptNumber; }
    void setAttemptNumber(int attempt) { m_attemptNumber = attempt; }
    
    QDateTime registeredAt() const { return m_registeredAt; }
    void setRegisteredAt(const QDateTime& dt) { m_registeredAt = dt; }

private:
    int m_id = 0;
    int m_studentId = 0;
    int m_courseId = 0;
    int m_semester = 0;
    int m_year = 0;
    QString m_status = "pending";
    int m_attemptNumber = 1;
    QDateTime m_registeredAt;
};

#endif // ENROLLMENT_H

