#ifndef ENROLLMENT_H
#define ENROLLMENT_H

#include <QString>
#include <QDateTime>

enum class EnrollmentStatus {
    Registered,
    Dropped,
    Pending
};

struct Enrollment {
    int id;
    int studentId;
    int courseId;
    int semester;
    int year;
    EnrollmentStatus status;
    int attemptNumber;
    QDateTime registeredAt;
    
    Enrollment() : id(0), studentId(0), courseId(0), 
                   semester(1), year(2024), status(EnrollmentStatus::Pending),
                   attemptNumber(1) {}
    
    QString statusString() const;
    static EnrollmentStatus statusFromString(const QString& str);
};

#endif // ENROLLMENT_H

