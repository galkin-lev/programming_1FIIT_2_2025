//
// Created by lv on 2/24/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_DB_H
#define LAB6_LOGIN_REGISTRATION_DB_H


#include <vector>
#include "../model/User.h"

bool initDatabase();
bool saveUser(const User& user);
std::vector<User> loadUsers();


#endif //LAB6_LOGIN_REGISTRATION_DB_H