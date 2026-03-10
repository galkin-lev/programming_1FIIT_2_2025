//
// Created by lv on 2/24/26.
//

#include "UserDao.h"

#include <QSqlQuery>
#include <qvariant.h>
#include <string>

#include "../model/User.h"
#include "../local_db/SimpleAES.h"

using namespace std;

UserDao::UserDao() : aes("abc")  {
}


bool UserDao::validateUserCredentials(const string& username, const string& password) {
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(QString::fromStdString(username));

    if (query.exec() && query.next()) {
        string storedPassword = query.value(0).toString().toStdString();
        return aes.decrypt(storedPassword) == password;
    }

    return false;
}


bool UserDao::saveUser(const User& user) {
    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO users (id, username, password) VALUES (?, ?, ?)");
    query.addBindValue(user.id == -1 ? QVariant() : user.id);
    query.addBindValue(QString::fromStdString(user.username));
    query.addBindValue(QString::fromStdString(aes.encrypt(user.password)));
    return query.exec();
}











vector<User> UserDao::loadAllUsers() {
    vector<User> users;
    QSqlQuery query("SELECT id, username, password FROM users ORDER BY id");

    while (query.next()) {
        User user;
        user.id = query.value(0).toInt();
        user.username = query.value(1).toString().toStdString();
        user.password = query.value(2).toString().toStdString();
        users.push_back(user);
    }

    return users;
}

optional<User> UserDao::findUserByUsername(const string& username) {
    QSqlQuery query;
    query.prepare("SELECT id, username, password FROM users WHERE username = ?");
    query.addBindValue(QString::fromStdString(username));

    if (query.exec() && query.next()) {
        User user;
        user.id = query.value(0).toInt();
        user.username = query.value(1).toString().toStdString();
        user.password = query.value(2).toString().toStdString();
        return user;
    }

    return nullopt;  // or return std::nullopt
}

optional<User> UserDao::findUserById(int id) {
    QSqlQuery query;
    query.prepare("SELECT id, username, password FROM users WHERE id = ?");
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        User user;
        user.id = query.value(0).toInt();
        user.username = query.value(1).toString().toStdString();
        user.password = query.value(2).toString().toStdString();
        return user;
    }

    return nullopt;
}

bool UserDao::deleteUser(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE id = ?");
    query.addBindValue(id);
    return query.exec();
}

bool UserDao::updateUser(const User& user) {
    QSqlQuery query;
    query.prepare("UPDATE users SET username = ?, password = ? WHERE id = ?");
    query.addBindValue(QString::fromStdString(user.username));
    query.addBindValue(QString::fromStdString(user.password));
    query.addBindValue(user.id);
    return query.exec();
}



