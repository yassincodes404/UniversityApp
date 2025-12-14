#include <QApplication>
#include "AppMainWindow/MainWindow.h"
#include "DAL/DatabaseManager.h"
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QSqlQuery>
#include <QFile>
#include <QProcessEnvironment>
#include <cstdlib>
#include <cstring>

bool initializeDatabase()
{
    DatabaseManager& db = DatabaseManager::instance();
    
    // Initialize database connection
    if (!db.initialize()) {
        QMessageBox::critical(nullptr, "Database Error", 
                            "Failed to initialize database connection.");
        return false;
    }
    
    QString appDir = QApplication::applicationDirPath();
    // Use the legacy SQLite schema that matches the current DAL/Models
    QString schemaPath = appDir + "/../db/sqlite_create_old.sql";
    
    // Try alternative paths for schema
    if (!QFile::exists(schemaPath)) {
        schemaPath = "db/sqlite_create_old.sql";
    }
    if (!QFile::exists(schemaPath)) {
        QString altPath = QStandardPaths::locate(QStandardPaths::AppDataLocation, "sqlite_create_old.sql");
        schemaPath = altPath;
    }
    // Also try absolute path from source
    if (!QFile::exists(schemaPath)) {
        schemaPath = "/home/yassin/UniversityApp-1/db/sqlite_create_old.sql";
    }
    
    // Always ensure schema exists (idempotent CREATE TABLE IF NOT EXISTS)
    if (QFile::exists(schemaPath)) {
        if (!db.executeScript(schemaPath)) {
            QMessageBox::warning(nullptr, "Database Warning", 
                               "Failed to create or update database schema. Some features may not work.");
        }
    } else {
        QMessageBox::warning(nullptr, "Database Warning", 
                           "Schema file not found. Database will be created on first use.");
    }
    
    // Always apply seed data (INSERT OR REPLACE makes it idempotent for our key rows)
    QString seedPath = appDir + "/../db/seed_data.sql";
    if (!QFile::exists(seedPath)) {
        seedPath = "db/seed_data.sql";
    }
    if (!QFile::exists(seedPath)) {
        seedPath = "/home/yassin/UniversityApp-1/db/seed_data.sql";
    }
    if (QFile::exists(seedPath)) {
        db.executeScript(seedPath);
    }

    // Lightweight migration for profile images: add column and change-request table if needed
    {
        QSqlQuery q(db.database());
        q.exec("ALTER TABLE students ADD COLUMN profile_image_path TEXT");
    }
    {
        QSqlQuery q(db.database());
        q.exec(
            "CREATE TABLE IF NOT EXISTS profile_image_change_requests ("
            "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  student_id INTEGER NOT NULL,"
            "  old_image_path TEXT,"
            "  new_image_path TEXT NOT NULL,"
            "  status TEXT CHECK(status IN ('pending','approved','rejected')) DEFAULT 'pending',"
            "  requested_at TEXT DEFAULT CURRENT_TIMESTAMP,"
            "  reviewed_at TEXT,"
            "  reviewed_by_admin_id INTEGER,"
            "  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE"
            ")"
        );
    }
    
    return true;
}

int main(int argc, char *argv[])
{
    // Fix DISPLAY for WSLg: WSLg provides X11 forwarding via :0
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    bool hasWslg = env.value("XDG_RUNTIME_DIR", "").contains("microsoft") || 
                   env.value("WAYLAND_DISPLAY", "").contains("wayland");
    
    if (hasWslg && env.contains("WAYLAND_DISPLAY")) {
        QString currentDisplay = env.value("DISPLAY", "");
        // If DISPLAY points to Windows IP and fails, use WSLg's local X11 forwarding
        if (!currentDisplay.isEmpty() && (currentDisplay.contains("10.255.255.254") || 
                                         !currentDisplay.startsWith(":"))) {
            // WSLg provides X11 via :0, set DISPLAY to :0
            setenv("DISPLAY", ":0", 1);
        } else if (currentDisplay.isEmpty() || currentDisplay == "NOT_SET") {
            // No DISPLAY set, use WSLg default
            setenv("DISPLAY", ":0", 1);
        }
    }
    
    QApplication app(argc, argv);
    
    app.setApplicationName("NCTU SIS");
    app.setOrganizationName("NCTU");
    
    if (!initializeDatabase()) {
        return 1;
    }
    
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
