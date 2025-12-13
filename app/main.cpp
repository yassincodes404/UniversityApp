#include <QApplication>
#include "AppMainWindow/MainWindow.h"
#include "DAL/DatabaseManager.h"
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QSqlQuery>
#include <QFile>
#include <QLoggingCategory>
#include <QProcessEnvironment>
#include <QFileInfo>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>
#include <QProcess>
#include <cstdlib>
#include <unistd.h>
#include <cstring>

// #region agent log
void debugLog(const QString& location, const QString& message, const QVariantMap& data = QVariantMap(), const QString& hypothesisId = "") {
    QFile logFile("/home/yassin/UniversityApp-1/.cursor/debug.log");
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&logFile);
        QJsonObject logEntry;
        logEntry["id"] = QString("log_%1_%2").arg(QDateTime::currentMSecsSinceEpoch()).arg(rand() % 10000);
        logEntry["timestamp"] = QDateTime::currentMSecsSinceEpoch();
        logEntry["location"] = location;
        logEntry["message"] = message;
        logEntry["sessionId"] = "debug-session";
        logEntry["runId"] = "run1";
        if (!hypothesisId.isEmpty()) logEntry["hypothesisId"] = hypothesisId;
        QJsonObject dataObj;
        for (auto it = data.begin(); it != data.end(); ++it) {
            dataObj[it.key()] = QJsonValue::fromVariant(it.value());
        }
        logEntry["data"] = dataObj;
        out << QJsonDocument(logEntry).toJson(QJsonDocument::Compact) << "\n";
        logFile.close();
    }
}
// #endregion

bool initializeDatabase()
{
    // #region agent log
    QVariantMap dbInit;
    dbInit["step"] = "START";
    debugLog("main.cpp:46", "Database initialization started", dbInit, "A");
    // #endregion
    
    DatabaseManager& db = DatabaseManager::instance();
    
    // Initialize database connection
    if (!db.initialize()) {
        // #region agent log
        QVariantMap dbError;
        dbError["step"] = "INIT_FAILED";
        debugLog("main.cpp:54", "Database initialization failed", dbError, "A");
        // #endregion
        QMessageBox::critical(nullptr, "Database Error", 
                            "Failed to initialize database connection.");
        return false;
    }
    
    // #region agent log
    QVariantMap dbConnected;
    dbConnected["step"] = "CONNECTED";
    debugLog("main.cpp:62", "Database connected", dbConnected, "A");
    // #endregion
    
    // Check if database is new (no tables exist)
    QSqlQuery checkQuery(db.database());
    checkQuery.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='students'");
    
    // #region agent log
    QVariantMap tableCheck;
    tableCheck["has_students_table"] = checkQuery.next();
    debugLog("main.cpp:70", "Table existence check", tableCheck, "B");
    // #endregion
    
    if (!checkQuery.next()) {
        // Database is new, create schema
        QString appDir = QApplication::applicationDirPath();
        QString schemaPath = appDir + "/../db/sqlite_create.sql";
        
        // #region agent log
        QVariantMap pathCheck;
        pathCheck["app_dir"] = appDir;
        pathCheck["path1"] = schemaPath;
        pathCheck["path1_exists"] = QFile::exists(schemaPath);
        debugLog("main.cpp:78", "Checking schema path 1", pathCheck, "C");
        // #endregion
        
        // Try alternative paths
        if (!QFile::exists(schemaPath)) {
            schemaPath = "db/sqlite_create.sql";
            // #region agent log
            QVariantMap pathCheck2;
            pathCheck2["path2"] = schemaPath;
            pathCheck2["path2_exists"] = QFile::exists(schemaPath);
            debugLog("main.cpp:85", "Checking schema path 2", pathCheck2, "C");
            // #endregion
        }
        if (!QFile::exists(schemaPath)) {
            QString altPath = QStandardPaths::locate(QStandardPaths::AppDataLocation, "sqlite_create.sql");
            schemaPath = altPath;
            // #region agent log
            QVariantMap pathCheck3;
            pathCheck3["path3"] = schemaPath;
            pathCheck3["path3_exists"] = QFile::exists(schemaPath);
            debugLog("main.cpp:92", "Checking schema path 3", pathCheck3, "C");
            // #endregion
        }
        
        // Also try absolute path from source
        if (!QFile::exists(schemaPath)) {
            schemaPath = "/home/yassin/UniversityApp-1/db/sqlite_create.sql";
            // #region agent log
            QVariantMap pathCheck4;
            pathCheck4["path4"] = schemaPath;
            pathCheck4["path4_exists"] = QFile::exists(schemaPath);
            debugLog("main.cpp:100", "Checking schema path 4 (absolute)", pathCheck4, "C");
            // #endregion
        }
        
        if (QFile::exists(schemaPath)) {
            // #region agent log
            QVariantMap scriptExec;
            scriptExec["schema_path"] = schemaPath;
            scriptExec["file_size"] = QFileInfo(schemaPath).size();
            debugLog("main.cpp:107", "Executing schema script", scriptExec, "D");
            // #endregion
            
            if (!db.executeScript(schemaPath)) {
                // #region agent log
                QVariantMap scriptError;
                scriptError["schema_path"] = schemaPath;
                scriptError["error"] = "EXECUTION_FAILED";
                debugLog("main.cpp:113", "Schema script execution failed", scriptError, "D");
                // #endregion
                QMessageBox::warning(nullptr, "Database Warning", 
                                   "Failed to create database schema. Some features may not work.");
            } else {
                // #region agent log
                QVariantMap scriptSuccess;
                scriptSuccess["schema_path"] = schemaPath;
                debugLog("main.cpp:120", "Schema script executed successfully", scriptSuccess, "D");
                // #endregion
                
                // Load seed data
                QString seedPath = appDir + "/../db/seed_data.sql";
                if (!QFile::exists(seedPath)) {
                    seedPath = "db/seed_data.sql";
                }
                if (!QFile::exists(seedPath)) {
                    seedPath = "/home/yassin/UniversityApp-1/db/seed_data.sql";
                }
                
                // #region agent log
                QVariantMap seedCheck;
                seedCheck["seed_path"] = seedPath;
                seedCheck["seed_exists"] = QFile::exists(seedPath);
                debugLog("main.cpp:133", "Checking seed data path", seedCheck, "E");
                // #endregion
                
                if (QFile::exists(seedPath)) {
                    db.executeScript(seedPath);
                    // #region agent log
                    debugLog("main.cpp:138", "Seed data script executed", QVariantMap(), "E");
                    // #endregion
                }
            }
        } else {
            // #region agent log
            QVariantMap notFound;
            notFound["error"] = "SCHEMA_FILE_NOT_FOUND";
            debugLog("main.cpp:144", "Schema file not found", notFound, "C");
            // #endregion
            QMessageBox::warning(nullptr, "Database Warning", 
                               "Schema file not found. Database will be created on first use.");
        }
    }
    
    // #region agent log
    QVariantMap dbComplete;
    dbComplete["step"] = "COMPLETE";
    debugLog("main.cpp:152", "Database initialization complete", dbComplete, "A");
    // #endregion
    
    return true;
}

int main(int argc, char *argv[])
{
    // #region agent log
    QVariantMap envData;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString display = env.value("DISPLAY", "NOT_SET");
    envData["DISPLAY"] = display;
    envData["WAYLAND_DISPLAY"] = env.value("WAYLAND_DISPLAY", "NOT_SET");
    envData["XDG_RUNTIME_DIR"] = env.value("XDG_RUNTIME_DIR", "NOT_SET");
    debugLog("main.cpp:63", "Application startup - environment check", envData, "A");
    // #endregion
    
    // #region agent log
    // Check if X11 server is reachable
    QVariantMap x11Check;
    if (!display.isEmpty() && display != "NOT_SET") {
        QString host = display.split(":")[0];
        QString displayNum = display.contains(":") ? display.split(":")[1].split(".")[0] : "0";
        x11Check["display_host"] = host;
        x11Check["display_num"] = displayNum;
        x11Check["display_full"] = display;
        
        // Try to check if X server is accessible
        QProcess xdpyinfo;
        xdpyinfo.start("xdpyinfo", QStringList() << "-display" << display);
        x11Check["xdpyinfo_available"] = xdpyinfo.waitForFinished(1000);
        if (x11Check["xdpyinfo_available"].toBool()) {
            x11Check["x11_connection_status"] = xdpyinfo.exitCode() == 0 ? "SUCCESS" : "FAILED";
            x11Check["x11_error"] = QString::fromUtf8(xdpyinfo.readAllStandardError());
        } else {
            x11Check["x11_connection_status"] = "TIMEOUT_OR_NOT_FOUND";
        }
    } else {
        x11Check["display_status"] = "NOT_SET";
    }
    debugLog("main.cpp:107", "X11 connection check", x11Check, "C");
    // #endregion
    
    // #region agent log
    // Check for WSLg
    QVariantMap wslgCheck;
    wslgCheck["wslg_runtime_dir"] = env.value("XDG_RUNTIME_DIR", "NOT_SET");
    wslgCheck["wayland_display"] = env.value("WAYLAND_DISPLAY", "NOT_SET");
    bool hasWslg = env.value("XDG_RUNTIME_DIR", "").contains("microsoft") || 
                   env.value("WAYLAND_DISPLAY", "").contains("wayland");
    wslgCheck["wslg_detected"] = hasWslg;
    debugLog("main.cpp:120", "WSLg detection", wslgCheck, "D");
    // #endregion
    
    // #region agent log
    // Fix DISPLAY for WSLg: WSLg provides X11 forwarding via :0
    QVariantMap platformFix;
    if (hasWslg && env.contains("WAYLAND_DISPLAY")) {
        QString currentDisplay = env.value("DISPLAY", "");
        // If DISPLAY points to Windows IP and fails, use WSLg's local X11 forwarding
        if (!currentDisplay.isEmpty() && (currentDisplay.contains("10.255.255.254") || 
                                         !currentDisplay.startsWith(":"))) {
            // WSLg provides X11 via :0, set DISPLAY to :0
            setenv("DISPLAY", ":0", 1);
            platformFix["action"] = "SET_DISPLAY_TO_LOCAL";
            platformFix["old_display"] = currentDisplay;
            platformFix["new_display"] = ":0";
            debugLog("main.cpp:152", "Setting DISPLAY to :0 for WSLg", platformFix, "D");
        } else if (currentDisplay.isEmpty() || currentDisplay == "NOT_SET") {
            // No DISPLAY set, use WSLg default
            setenv("DISPLAY", ":0", 1);
            platformFix["action"] = "SET_DISPLAY_TO_LOCAL";
            platformFix["old_display"] = "NOT_SET";
            platformFix["new_display"] = ":0";
            debugLog("main.cpp:160", "Setting DISPLAY to :0 for WSLg (was unset)", platformFix, "D");
        }
    }
    // #endregion
    
    QApplication app(argc, argv);
    
    // #region agent log
    QVariantMap appInit;
    appInit["platform_name"] = QApplication::platformName();
    appInit["qt_version"] = QT_VERSION_STR;
    appInit["application_state"] = "INITIALIZED";
    debugLog("main.cpp:128", "QApplication initialized", appInit, "E");
    // #endregion
    
    app.setApplicationName("NCTU SIS");
    app.setOrganizationName("NCTU");
    
    if (!initializeDatabase()) {
        // #region agent log
        debugLog("main.cpp:109", "Database initialization failed", QVariantMap(), "F");
        // #endregion
        return 1;
    }
    
    // #region agent log
    debugLog("main.cpp:114", "Creating MainWindow", QVariantMap(), "G");
    // #endregion
    
    MainWindow mainWindow;
    
    // #region agent log
    debugLog("main.cpp:118", "Calling mainWindow.show()", QVariantMap(), "H");
    // #endregion
    
    mainWindow.show();
    
    // #region agent log
    debugLog("main.cpp:122", "Entering event loop", QVariantMap(), "I");
    // #endregion
    
    return app.exec();
}

