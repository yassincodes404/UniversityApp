#ifndef ADMINREPO_H
#define ADMINREPO_H

#include <QString>
#include <optional>

class QSqlQuery;

struct Admin {
    int id = 0;
    QString name;
    QString email;
    QString passwordHash;
    QString salt;
};

class AdminRepo
{
public:
    AdminRepo();
    
    std::optional<Admin> findById(int id);
    std::optional<Admin> findByEmail(const QString& email);
    bool save(Admin& admin);
    bool update(const Admin& admin);

private:
    Admin mapFromQuery(const QSqlQuery& query);
};

#endif // ADMINREPO_H

