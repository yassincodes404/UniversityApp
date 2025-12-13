#ifndef EXAM_H
#define EXAM_H

#include <QString>
#include <QDate>
#include <QDateTime>

class Exam
{
public:
    Exam();
    
    int id() const { return m_id; }
    void setId(int id) { m_id = id; }
    
    int courseId() const { return m_courseId; }
    void setCourseId(int id) { m_courseId = id; }
    
    QString examType() const { return m_examType; }
    void setExamType(const QString& type) { m_examType = type; }
    
    QDate examDate() const { return m_examDate; }
    void setExamDate(const QDate& date) { m_examDate = date; }
    
    int year() const { return m_year; }
    void setYear(int year) { m_year = year; }
    
    int semester() const { return m_semester; }
    void setSemester(int sem) { m_semester = sem; }
    
    int durationMinutes() const { return m_durationMinutes; }
    void setDurationMinutes(int minutes) { m_durationMinutes = minutes; }
    
    QString venue() const { return m_venue; }
    void setVenue(const QString& venue) { m_venue = venue; }
    
    QDateTime createdAt() const { return m_createdAt; }
    void setCreatedAt(const QDateTime& dt) { m_createdAt = dt; }

private:
    int m_id = 0;
    int m_courseId = 0;
    QString m_examType;
    QDate m_examDate;
    int m_year = 0;
    int m_semester = 0;
    int m_durationMinutes = 0;
    QString m_venue;
    QDateTime m_createdAt;
};

#endif // EXAM_H

