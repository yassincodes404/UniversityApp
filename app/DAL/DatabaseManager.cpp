#include "DatabaseManager.h"
#include <QSqlError>
#include <QDebug>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::open(const DbConfig& config)
{
    if (m_isOpen) {
        close();
    }
    
    m_db = QSqlDatabase::addDatabase("QMYSQL", "nctu_sis_connection");
    m_db.setHostName(config.hostName);
    m_db.setDatabaseName(config.databaseName);
    m_db.setUserName(config.userName);
    m_db.setPassword(config.password);
    m_db.setPort(config.port);
    
    m_isOpen = m_db.open();
    
    if (!m_isOpen) {
        qDebug() << "Database error:" << m_db.lastError().text();
    }
    
    return m_isOpen;
}

void DatabaseManager::close()
{
    if (m_isOpen) {
        m_db.close();
        m_isOpen = false;
    }
}

bool DatabaseManager::isOpen() const
{
    return m_isOpen;
}

QSqlDatabase& DatabaseManager::database()
{
    return m_db;
}

bool DatabaseManager::beginTransaction()
{
    return m_db.transaction();
}

bool DatabaseManager::commitTransaction()
{
    return m_db.commit();
}

bool DatabaseManager::rollbackTransaction()
{
    return m_db.rollback();
}

