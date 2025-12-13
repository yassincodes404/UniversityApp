#include "DatabaseManager.h"
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::initialize(const QString& dbPath)
{
    if (m_initialized && m_db.isOpen()) {
        return true;
    }
    
    // Use QStandardPaths for better cross-platform support
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    QString fullPath = QDir(dataPath).filePath(dbPath);
    
    m_db = QSqlDatabase::addDatabase("QSQLITE", "NCTU_SIS_CONNECTION");
    m_db.setDatabaseName(fullPath);
    
    if (!m_db.open()) {
        qDebug() << "Database error:" << m_db.lastError().text();
        return false;
    }
    
    // Enable foreign keys
    QSqlQuery query(m_db);
    if (!query.exec("PRAGMA foreign_keys = ON")) {
        qDebug() << "Failed to enable foreign keys:" << query.lastError().text();
        return false;
    }
    
    m_initialized = true;
    qDebug() << "Database initialized at:" << fullPath;
    return true;
}

bool DatabaseManager::executeScript(const QString& scriptPath)
{
    QFile file(scriptPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open script file:" << scriptPath;
        return false;
    }
    
    QTextStream in(&file);
    QString script = in.readAll();
    file.close();
    
    // Better SQL parsing: handle multi-line statements and comments
    QStringList statements;
    QString currentStatement;
    bool inString = false;
    char stringChar = 0;
    
    for (int i = 0; i < script.length(); ++i) {
        QChar c = script[i];
        QChar next = (i + 1 < script.length()) ? script[i + 1] : QChar();
        
        // Handle string literals
        if (!inString && (c == '\'' || c == '"')) {
            inString = true;
            stringChar = c.toLatin1();
            currentStatement += c;
        } else if (inString && c == stringChar) {
            // Check for escaped quotes
            if (next == stringChar) {
                currentStatement += c;
                currentStatement += next;
                ++i; // Skip next char
            } else {
                inString = false;
                currentStatement += c;
            }
        } else if (!inString && c == ';') {
            // End of statement
            QString trimmed = currentStatement.trimmed();
            if (!trimmed.isEmpty() && !trimmed.startsWith("--")) {
                statements.append(trimmed);
            }
            currentStatement.clear();
        } else if (!inString && c == '-' && next == '-') {
            // Skip single-line comments
            while (i < script.length() && script[i] != '\n') {
                ++i;
            }
        } else if (!inString && c == '/' && next == '*') {
            // Skip multi-line comments
            i += 2;
            while (i + 1 < script.length()) {
                if (script[i] == '*' && script[i + 1] == '/') {
                    i += 2;
                    break;
                }
                ++i;
            }
        } else {
            currentStatement += c;
        }
    }
    
    // Add last statement if exists
    QString trimmed = currentStatement.trimmed();
    if (!trimmed.isEmpty() && !trimmed.startsWith("--")) {
        statements.append(trimmed);
    }
    
    beginTransaction();
    
    int statementIndex = 0;
    for (const QString& statement : statements) {
        QString trimmed = statement.trimmed();
        if (trimmed.isEmpty()) {
            continue;
        }
        
        QSqlQuery query(m_db);
        if (!query.exec(trimmed)) {
            qDebug() << "Script execution error at statement" << statementIndex << ":" << query.lastError().text();
            qDebug() << "Statement:" << trimmed.left(200);
            rollbackTransaction();
            return false;
        }
        ++statementIndex;
    }
    
    commitTransaction();
    return true;
}

QSqlDatabase& DatabaseManager::database()
{
    return m_db;
}

bool DatabaseManager::isOpen() const
{
    return m_db.isOpen();
}

void DatabaseManager::close()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
    m_initialized = false;
}

bool DatabaseManager::beginTransaction()
{
    QSqlQuery query(m_db);
    return query.exec("BEGIN TRANSACTION");
}

bool DatabaseManager::commitTransaction()
{
    QSqlQuery query(m_db);
    return query.exec("COMMIT");
}

bool DatabaseManager::rollbackTransaction()
{
    QSqlQuery query(m_db);
    return query.exec("ROLLBACK");
}

DatabaseManager::~DatabaseManager()
{
    close();
}

