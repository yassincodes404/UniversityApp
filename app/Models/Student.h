#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QDate>
#include <optional>

enum class StudentStatus {
    Active,
    Graduated,
    Suspended
};

enum class Gender {
    Male,
    Female,
    Other
};

struct Student {
    int id;
    QString studentCode;
    QString name;
    QString email;
    QString phone;
    QString passwordHash;
    QString salt;
    int programId;
    int departmentId;
    QString nationalId;
    QDate birthDate;
    Gender gender;
    QString seatNumber;
    StudentStatus status;
    int level;
    int creditsCompleted;
    double gpa;
    bool mustChangePassword;
    
    Student() : id(0), programId(0), departmentId(0), 
                gender(Gender::Other), status(StudentStatus::Active),
                level(1), creditsCompleted(0), gpa(0.0), mustChangePassword(true) {}
    
    QString statusString() const;
    QString genderString() const;
    static StudentStatus statusFromString(const QString& str);
    static Gender genderFromString(const QString& str);
};

#endif // STUDENT_H

