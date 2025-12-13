#ifndef ENROLLMENTREPO_H
#define ENROLLMENTREPO_H

#include "../Models/Enrollment.h"
#include <QList>
#include <optional>

class QSqlQuery;

class EnrollmentRepo
{
public:
    EnrollmentRepo();
    
    std::optional<Enrollment> findById(int id);
    QList<Enrollment> findByStudent(int studentId);
    QList<Enrollment> findByStudentAndSemester(int studentId, int semester, int year);
    QList<Enrollment> findByCourse(int courseId);
    QList<Enrollment> findPending();
    
    bool save(Enrollment& enrollment);
    bool update(const Enrollment& enrollment);
    bool remove(int id);
    bool approve(int enrollmentId);
    bool reject(int enrollmentId);

private:
    Enrollment mapFromQuery(const QSqlQuery& query);
};

#endif // ENROLLMENTREPO_H

