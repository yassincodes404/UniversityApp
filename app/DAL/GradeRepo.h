#ifndef GRADEREPO_H
#define GRADEREPO_H

#include "../Models/Grade.h"
#include <QList>
#include <optional>

class QSqlQuery;

class GradeRepo
{
public:
    GradeRepo();
    
    std::optional<Grade> findById(int id);
    std::optional<Grade> findByEnrollment(int enrollmentId);
    std::optional<Grade> findByStudentAndCourse(int studentId, int courseId);
    QList<Grade> findByStudent(int studentId);
    QList<Grade> findByCourse(int courseId);
    
    bool save(Grade& grade);
    bool update(const Grade& grade);
    bool remove(int id);
    bool publishGrades(int courseId);

private:
    Grade mapFromQuery(const QSqlQuery& query);
};

#endif // GRADEREPO_H

