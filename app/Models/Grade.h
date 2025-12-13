#ifndef GRADE_H
#define GRADE_H

#include <QString>
#include <QDateTime>

class Grade
{
public:
    Grade();
    
    int id() const { return m_id; }
    void setId(int id) { m_id = id; }
    
    int enrollmentId() const { return m_enrollmentId; }
    void setEnrollmentId(int id) { m_enrollmentId = id; }
    
    int studentId() const { return m_studentId; }
    void setStudentId(int id) { m_studentId = id; }
    
    int courseId() const { return m_courseId; }
    void setCourseId(int id) { m_courseId = id; }
    
    double midterm1() const { return m_midterm1; }
    void setMidterm1(double score) { m_midterm1 = score; }
    
    double midterm2() const { return m_midterm2; }
    void setMidterm2(double score) { m_midterm2 = score; }
    
    double finalExam() const { return m_finalExam; }
    void setFinalExam(double score) { m_finalExam = score; }
    
    double practical() const { return m_practical; }
    void setPractical(double score) { m_practical = score; }
    
    double assignments() const { return m_assignments; }
    void setAssignments(double score) { m_assignments = score; }
    
    double total() const { return m_total; }
    void setTotal(double total) { m_total = total; }
    
    QString letterGrade() const { return m_letterGrade; }
    void setLetterGrade(const QString& grade) { m_letterGrade = grade; }
    
    QString resultStatus() const { return m_resultStatus; }
    void setResultStatus(const QString& status) { m_resultStatus = status; }
    
    bool isPublished() const { return m_isPublished; }
    void setIsPublished(bool published) { m_isPublished = published; }
    
    QDateTime recordedAt() const { return m_recordedAt; }
    void setRecordedAt(const QDateTime& dt) { m_recordedAt = dt; }

private:
    int m_id = 0;
    int m_enrollmentId = 0;
    int m_studentId = 0;
    int m_courseId = 0;
    double m_midterm1 = 0.0;
    double m_midterm2 = 0.0;
    double m_finalExam = 0.0;
    double m_practical = 0.0;
    double m_assignments = 0.0;
    double m_total = 0.0;
    QString m_letterGrade;
    QString m_resultStatus = "fail";
    bool m_isPublished = false;
    QDateTime m_recordedAt;
};

#endif // GRADE_H

