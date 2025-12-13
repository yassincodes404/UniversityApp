#ifndef ENROLLMENTREPO_H
#define ENROLLMENTREPO_H

#include "../Models/Enrollment.h"
#include <QList>
#include <optional>

class EnrollmentRepo
{
public:
    EnrollmentRepo();
    
    std::optional<Enrollment> findById(int id);
    QList<Enrollment> findByStudent(int studentId);
    QList<Enrollment> findByCourse(int courseId);
    QList<Enrollment> findByStudentAndSemester(int studentId, int semester, int year);
    
    bool save(const Enrollment& enrollment);
    bool update(const Enrollment& enrollment);
    bool remove(int id);
    bool approveEnrollment(int id);
    bool rejectEnrollment(int id);
};

#endif // ENROLLMENTREPO_H

