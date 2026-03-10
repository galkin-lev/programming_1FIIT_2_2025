//
// Created by lv on 2/24/26.
//

#include "db.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>  // YOU NEED THIS FOR QSqlError
#include "../../../logger/logger.h"

bool initDatabase() {


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    LOG_INFO("Opening database: " + db.databaseName().toStdString());

    if (!db.open()) {
        LOG_FATAL("Cannot open database: " + db.lastError().text().toStdString());
        return false;
    }

    QSqlQuery query;
    bool success = query.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL
        )
    )");

    if (success) {
        LOG_INFO("Database initialized successfully");
    } else {
        LOG_ERROR("Failed to create table: " + query.lastError().text().toStdString());
    }

    return success;


    // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // db.setDatabaseName("users.db");
    //
    // if (!db.open()) {
    //     qCritical() << "Cannot open database";
    //     return false;
    // }
    //
    // QSqlQuery query;
    // query.exec(R"(
    //     CREATE TABLE IF NOT EXISTS users (
    //         id INTEGER PRIMARY KEY AUTOINCREMENT,
    //         username TEXT NOT NULL UNIQUE,
    //         password TEXT NOT NULL
    //     )
    // )");
    //
    // LOG_INFO("database successfully initialized");
    // return true;
}