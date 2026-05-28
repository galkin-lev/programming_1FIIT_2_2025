//
// Created by lv on 2/24/26.
//

// db.cpp
#include "db.h"

#include "../../../logger/logger.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QCoreApplication>

static QSqlDatabase database;
static bool isInitialized = false;

// Helper function to get the database path consistently
static QString getDatabasePath() {
    // Try multiple locations in order of preference

    // 1. Check if there's an existing database in the current working directory
    QString cwdPath = QDir::current().absoluteFilePath("users.db");
    if (QFile::exists(cwdPath)) {
        LOG_DEBUG("Found database in current directory: " + cwdPath.toStdString());
        return cwdPath;
    }

    // 2. Check in the project root (parent of build directory)
    QString appPath = QCoreApplication::applicationDirPath();
    QDir appDir(appPath);

    // Go up from cmake-build-debug or cmake-build-default to project root
    if (appDir.dirName().contains("cmake-build-")) {
        appDir.cdUp();
    }
    QString projectPath = appDir.absoluteFilePath("users.db");
    if (QFile::exists(projectPath)) {
        LOG_DEBUG("Found database in project root: " + projectPath.toStdString());
        return projectPath;
    }

    // 3. Default: use the application directory
    QString defaultPath = QCoreApplication::applicationDirPath() + "/users.db";
    LOG_DEBUG("Using default database path: " + defaultPath.toStdString());
    return defaultPath;
}

bool initDatabase() {
    if (isInitialized) {
        LOG_WARN("Database already initialized, returning existing connection");
        return database.isOpen();
    }

    // Use a named connection
    const QString connectionName = "photo_app_connection";

    // Remove existing connection if it exists (to avoid duplicate)
    if (QSqlDatabase::contains(connectionName)) {
        LOG_DEBUG("Removing existing database connection");
        QSqlDatabase::removeDatabase(connectionName);
    }

    // Create new connection
    database = QSqlDatabase::addDatabase("QSQLITE", connectionName);

    // Use the consistent database path
    QString dbPath = getDatabasePath();
    database.setDatabaseName(dbPath);

    LOG_INFO("Opening database: " + database.databaseName().toStdString());

    if (!database.open()) {
        LOG_FATAL("Cannot open database: " + database.lastError().text().toStdString());
        return false;
    }

    isInitialized = true;

    // Create tables with avatar_uri column
    QSqlQuery query(database);
    bool success = query.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL,
            avatar_uri TEXT
        )
    )");

    if (!success) {
        LOG_ERROR("Failed to create table: " + query.lastError().text().toStdString());
        return false;
    }

    // Add avatar_uri column if it doesn't exist (for existing databases)
    if (!query.exec("ALTER TABLE users ADD COLUMN avatar_uri TEXT")) {
        // Column might already exist, that's fine
        LOG_DEBUG("Avatar column already exists or could not be added");
    }

    // Create index
    query.exec("CREATE INDEX IF NOT EXISTS idx_username ON users(username)");

    // Verify table and count users
    if (query.exec("SELECT COUNT(*) FROM users")) {
        if (query.next()) {
            int count = query.value(0).toInt();
            LOG_INFO("Database initialized successfully. Users in database: " + std::to_string(count));
        }
    } else {
        LOG_INFO("Database initialized successfully (users table empty or not yet created)");
    }

    return true;
}

QSqlDatabase getDatabase() {
    if (!isInitialized || !database.isOpen()) {
        if (!initDatabase()) {
            LOG_ERROR("Failed to initialize database");
            return QSqlDatabase();  // Return invalid database
        }
    }
    // Return a copy of the database connection
    return QSqlDatabase::database("photo_app_connection");
}

void closeDatabase() {
    if (database.isOpen()) {
        database.close();
        LOG_INFO("Database closed");
    }

    // Remove the connection
    const QString connectionName = "photo_app_connection";
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    isInitialized = false;
}