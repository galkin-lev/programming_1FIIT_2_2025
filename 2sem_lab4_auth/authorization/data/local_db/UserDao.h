//
// Created by lv on 2/24/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_USERDAO_H
#define LAB6_LOGIN_REGISTRATION_USERDAO_H
#include <optional>
#include <vector>

#include "SimpleAES.h"
#include "../model/User.h"

class UserDao {

protected:
    string encryptionPassword = "abc";
    SimpleAES aes;

public:
    UserDao();
    bool saveUser(const User& user);
    vector<User> loadAllUsers();
    optional<User> findUserByUsername(const string& username);
    optional<User> findUserById(int id);
    bool deleteUser(int id);
    bool updateUser(const User& user);
    bool validateUserCredentials(const string& username, const string& password);

    // string md5(const string& input);

};


#endif //LAB6_LOGIN_REGISTRATION_USERDAO_H