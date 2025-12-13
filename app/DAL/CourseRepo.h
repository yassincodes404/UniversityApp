#ifndef COURSEREPO_H
#define COURSEREPO_H

#include "../Models/Course.h"
#include <QList>
#include <QString>
#include <optional>

class QSqlQuery;

class CourseRepo
{
public:
    CourseRepo();
    
    std::optional<Course> findById(int id);
    std::optional<Course> findByCode(const QString& code);
    QList<Course> findAll();
    QList<Course> findByProgram(int programId);
    QList<Course> findByYearAndSemester(int year, const QString& semester);
    
    bool save(Course& course);
    bool update(const Course& course);
    bool remove(int id);

private:
    Course mapFromQuery(const QSqlQuery& query);
};

#endif // COURSEREPO_H

