//
// Created by lv on 2/24/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_USERDAO_H
#define LAB6_LOGIN_REGISTRATION_USERDAO_H
#include <optional>
#include <vector>

#include "../SimpleAES.h"
#include "../../../../core/model/User.h"
#include <memory>
#include <QSqlQuery>




class SimpleAES;

class UserDao {
public:
    UserDao();
    ~UserDao();

    // CRUD operations
    bool saveUser(const Core::Entities::User& user);
    bool updateUser(const Core::Entities::User& user);
    bool deleteUser(int userId);
    bool addPhotoToUser(int userId, const Core::Entities::Photo& photo);
    bool removePhotoFromUser(int userId, int photoId);
    bool updateUserAvatar(int userId, const QString& photoUri);

    bool clearAllUsers();


    // Query operations
    std::optional<Core::Entities::User> findUserByUsername(const QString& username);
    std::optional<Core::Entities::User> findUserById(int userId);
    std::vector<Core::Entities::User> loadAllUsers();
    bool userExists(const QString& username);
    bool validateUserCredentials(const QString& username, const QString& password);

    // JSON export/import
    bool exportToJson(const QString& filePath = "users_export.json");
    bool importFromJson(const QString& filePath = "users_export.json");

private:
    // Password helpers
    QString encryptPassword(const QString& password);
    QString decryptPassword(const QString& encryptedHex);
    QString hashPassword(const QString& password);

    // Database helpers
    bool ensureDatabaseOpen();
    Core::Entities::User extractUserFromQuery(QSqlQuery& query);

    QSqlDatabase m_database;
    std::unique_ptr<SimpleAES> m_aes;
    bool m_isInitialized;
};




#endif //LAB6_LOGIN_REGISTRATION_USERDAO_H