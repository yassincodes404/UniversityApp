#ifndef COURSEREPO_H
#define COURSEREPO_H

#include "../Models/Course.h"
#include <QString>
#include <optional>
#include <QList>

class CourseRepo
{
public:
    CourseRepo();
    
    std::optional<Course> findByCode(const QString& code);
    std::optional<Course> findById(int id);
    QList<Course> findAll();
    QList<Course> findByProgram(int programId);
    QList<Course> findByYearAndSemester(int year, Semester semester);
    
    bool save(const Course& course);
    bool update(const Course& course);
    bool remove(int id);
};

#endif // COURSEREPO_H

