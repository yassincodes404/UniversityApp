#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QObject>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    static DatabaseManager& instance();
    
    bool initialize(const QString& dbPath = "nctu_sis.db");
    bool executeScript(const QString& scriptPath);
    QSqlDatabase& database();
    bool isOpen() const;
    void close();
    
    // Transaction support
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

private:
    DatabaseManager() = default;
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    
    QSqlDatabase m_db;
    bool m_initialized = false;
};

#endif // DATABASEMANAGER_H

