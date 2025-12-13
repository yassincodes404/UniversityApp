#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QDate>
#include <optional>

class Student
{
public:
    Student();
    Student(int id, const QString& studentCode, const QString& name);
    
    int id() const { return m_id; }
    void setId(int id) { m_id = id; }
    
    QString studentCode() const { return m_studentCode; }
    void setStudentCode(const QString& code) { m_studentCode = code; }
    
    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }
    
    QString email() const { return m_email; }
    void setEmail(const QString& email) { m_email = email; }
    
    QString phone() const { return m_phone; }
    void setPhone(const QString& phone) { m_phone = phone; }
    
    QString passwordHash() const { return m_passwordHash; }
    void setPasswordHash(const QString& hash) { m_passwordHash = hash; }
    
    QString salt() const { return m_salt; }
    void setSalt(const QString& salt) { m_salt = salt; }
    
    int programId() const { return m_programId; }
    void setProgramId(int id) { m_programId = id; }
    
    int departmentId() const { return m_departmentId; }
    void setDepartmentId(int id) { m_departmentId = id; }
    
    QString nationalId() const { return m_nationalId; }
    void setNationalId(const QString& id) { m_nationalId = id; }
    
    QDate birthDate() const { return m_birthDate; }
    void setBirthDate(const QDate& date) { m_birthDate = date; }
    
    QString gender() const { return m_gender; }
    void setGender(const QString& gender) { m_gender = gender; }
    
    QString seatNumber() const { return m_seatNumber; }
    void setSeatNumber(const QString& seat) { m_seatNumber = seat; }
    
    QString status() const { return m_status; }
    void setStatus(const QString& status) { m_status = status; }
    
    int level() const { return m_level; }
    void setLevel(int level) { m_level = level; }
    
    int creditsCompleted() const { return m_creditsCompleted; }
    void setCreditsCompleted(int credits) { m_creditsCompleted = credits; }
    
    double gpa() const { return m_gpa; }
    void setGpa(double gpa) { m_gpa = gpa; }
    
    bool mustChangePassword() const { return m_mustChangePassword; }
    void setMustChangePassword(bool must) { m_mustChangePassword = must; }

private:
    int m_id = 0;
    QString m_studentCode;
    QString m_name;
    QString m_email;
    QString m_phone;
    QString m_passwordHash;
    QString m_salt;
    int m_programId = 0;
    int m_departmentId = 0;
    QString m_nationalId;
    QDate m_birthDate;
    QString m_gender = "Other";
    QString m_seatNumber;
    QString m_status = "active";
    int m_level = 1;
    int m_creditsCompleted = 0;
    double m_gpa = 0.0;
    bool m_mustChangePassword = false;
};

#endif // STUDENT_H

