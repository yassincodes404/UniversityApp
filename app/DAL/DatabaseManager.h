#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>
#include <memory>

struct DbConfig {
    QString hostName;
    QString databaseName;
    QString userName;
    QString password;
    int port;
    
    DbConfig() : port(3306) {}
};

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    
    bool open(const DbConfig& config);
    void close();
    bool isOpen() const;
    QSqlDatabase& database();
    
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    
private:
    DatabaseManager() = default;
    ~DatabaseManager() = default;
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    
    QSqlDatabase m_db;
    bool m_isOpen;
};

#endif // DATABASEMANAGER_H

