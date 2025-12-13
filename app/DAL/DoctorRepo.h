#ifndef DOCTORREPO_H
#define DOCTORREPO_H

#include "../Models/Doctor.h"
#include <QList>
#include <QString>
#include <optional>

class QSqlQuery;

class DoctorRepo
{
public:
    DoctorRepo();
    
    std::optional<Doctor> findById(int id);
    std::optional<Doctor> findByEmail(const QString& email);
    QList<Doctor> findAll();
    QList<Doctor> findByDepartment(int departmentId);
    
    bool save(Doctor& doctor);
    bool update(const Doctor& doctor);
    bool remove(int id);

private:
    Doctor mapFromQuery(const QSqlQuery& query);
};

#endif // DOCTORREPO_H

