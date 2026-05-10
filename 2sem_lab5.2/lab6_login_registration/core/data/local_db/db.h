//
// Created by lv on 2/24/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_DB_H
#define LAB6_LOGIN_REGISTRATION_DB_H


#include <vector>
#include "../../model/User.h"


#include <QSqlDatabase>


bool initDatabase();
void closeDatabase();
QSqlDatabase getDatabase();


#endif //LAB6_LOGIN_REGISTRATION_DB_H