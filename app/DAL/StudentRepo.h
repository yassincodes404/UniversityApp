#ifndef STUDENTREPO_H
#define STUDENTREPO_H

#include "../Models/Student.h"
#include <QList>
#include <QString>
#include <optional>

class QSqlQuery;

class StudentRepo
{
public:
    StudentRepo();
    
    std::optional<Student> findById(int id);
    std::optional<Student> findByCode(const QString& code);
    std::optional<Student> findByEmail(const QString& email);
    QList<Student> findAll();
    QList<Student> findByProgram(int programId);
    QList<Student> findByDepartment(int departmentId);
    
    bool save(Student& student);
    bool update(const Student& student);
    bool remove(int id);
    
    bool exists(const QString& studentCode);

private:
    Student mapFromQuery(const QSqlQuery& query);
};

#endif // STUDENTREPO_H

