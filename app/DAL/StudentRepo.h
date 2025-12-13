#ifndef STUDENTREPO_H
#define STUDENTREPO_H

#include "../Models/Student.h"
#include <QString>
#include <optional>
#include <QList>

class StudentRepo
{
public:
    StudentRepo();
    
    std::optional<Student> findByCode(const QString& code);
    std::optional<Student> findById(int id);
    QList<Student> findAll();
    QList<Student> findByProgram(int programId);
    QList<Student> findByDepartment(int departmentId);
    
    bool save(const Student& student);
    bool update(const Student& student);
    bool remove(int id);
    
    bool updatePassword(int id, const QString& newPasswordHash, const QString& newSalt);
    bool updateGPA(int id, double gpa, int creditsCompleted);
};

#endif // STUDENTREPO_H

