//
// Created by lv on 2/24/26.
//

#include "../dao/UserDao.h"

#include <QSqlQuery>
#include <qvariant.h>
#include <string>

// #include "../../../core/entities/User.h"
#include "../SimpleAES.h"

#include "../../../../logger/logger.h"
#include <QSqlError>
#include <QCryptographicHash>

#include <QJsonArray>

// ==================== Constructor & Setup ====================

#include "../dao//UserDao.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
// #include "../../../core/entities/User.h"
#include <QFile>
#include <qjsonobject.h>

#include "../SimpleAES.h"
#include "../db.h"
#include "../../../../logger/logger.h"
#include "core/model/User.h"

UserDao::UserDao()
    : m_aes(std::make_unique<SimpleAES>("your-secret-key-here"))
    , m_isInitialized(false) {
    // Don't open database here - just ensure we can get it when needed
    LOG_DEBUG("UserDao created");
}

UserDao::~UserDao() = default;

bool UserDao::ensureDatabaseOpen() {
    // Check if we already have a valid database connection
    if (m_database.isOpen()) {
        return true;
    }

    // Get the database from the global db module
    try {
        m_database = getDatabase();  // This should work now
        if (m_database.isOpen()) {
            LOG_DEBUG("Database connection acquired successfully");
            m_isInitialized = true;
            return true;
        } else {
            LOG_ERROR("Database connection obtained but not open");
            return false;
        }
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to get database: " + std::string(e.what()));
        return false;
    }
}
// ==================== Password Helpers ====================

QString UserDao::encryptPassword(const QString& password) {
    std::string passwordStd = password.toStdString();
    std::string encryptedHex = m_aes->encrypt(passwordStd);
    return QString::fromStdString(encryptedHex);
}

QString UserDao::decryptPassword(const QString& encryptedHex) {
    std::string encryptedStd = encryptedHex.toStdString();
    std::string decrypted = m_aes->decrypt(encryptedStd);
    return QString::fromStdString(decrypted);
}

QString UserDao::hashPassword(const QString& password) {
    QByteArray hash = QCryptographicHash::hash(
        password.toUtf8(),
        QCryptographicHash::Sha256
    );
    return QString(hash.toHex());
}

// ==================== CRUD Operations ====================

bool UserDao::saveUser(const Core::Entities::User& user) {
    if (!ensureDatabaseOpen()) return false;

     QSqlQuery query(m_database);

    LOG_DEBUG("Saving user: " + user.getUsername().toStdString());
    LOG_DEBUG("User ID: " + std::to_string(user.getId()));

    // Get the password hash from user entity
    QString passwordHash = user.getPasswordHash();
    LOG_DEBUG("Password hash length: " + std::to_string(passwordHash.length()));

    // Encrypt the password hash before storing
    QString encryptedPassword = encryptPassword(passwordHash);
    LOG_DEBUG("Encrypted password length: " + std::to_string(encryptedPassword.length()));

    if (user.getId() == -1) {
        // Insert new user - only username and password columns
        query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
        LOG_DEBUG("Preparing INSERT query");
    } else {
        // Update existing user
        query.prepare("UPDATE users SET username = :username, password = :password WHERE id = :id");
        query.bindValue(":id", user.getId());
        LOG_DEBUG("Preparing UPDATE query for ID: " + std::to_string(user.getId()));
    }

    query.bindValue(":username", user.getUsername());
    query.bindValue(":password", encryptedPassword);

    // Log bound values for debugging
    LOG_DEBUG("Bound values count: " + std::to_string(query.boundValues().size()));
    LOG_DEBUG("Query: " + query.lastQuery().toStdString());

    if (!query.exec()) {
        LOG_ERROR("Failed to save user: " + query.lastError().text().toStdString());
        LOG_ERROR("Query was: " + query.lastQuery().toStdString());
        return false;
    }

    LOG_INFO("User saved successfully: " + user.getUsername().toStdString());
    return true;
}

bool UserDao::updateUser(const Core::Entities::User& user) {
    return saveUser(user);
}

bool UserDao::deleteUser(int userId) {
    if (!ensureDatabaseOpen()) return false;

     QSqlQuery query(m_database);
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", userId);

    if (!query.exec()) {
        LOG_ERROR("Failed to delete user: " + query.lastError().text().toStdString());
        return false;
    }

    LOG_INFO("User deleted with id: " + std::to_string(userId));
    return true;
}

bool UserDao::addPhotoToUser(int userId, const Core::Entities::Photo& photo) {
    QSqlQuery query;
    query.prepare("INSERT INTO photos (user_id, uri, created_at) VALUES (?, ?, ?)");
    query.addBindValue(userId);
    query.addBindValue(photo.getUri());  // Directly use QString
    query.addBindValue(QDateTime::currentDateTime());
    return query.exec();
}

bool UserDao::updateUserAvatar(int userId, const QString& photoUri) {
    if (!ensureDatabaseOpen()) return false;

    QSqlQuery query(m_database);  // Use m_database, not default constructor
    query.prepare("UPDATE users SET avatar_uri = :avatar_uri WHERE id = :id");
    query.bindValue(":avatar_uri", photoUri);
    query.bindValue(":id", userId);

    if (!query.exec()) {
        LOG_ERROR("Failed to update avatar: " + query.lastError().text().toStdString());
        return false;
    }

    LOG_DEBUG("Avatar updated for user: " + std::to_string(userId));
    return true;
}

bool UserDao::removePhotoFromUser(int userId, int photoId) {
    if (!ensureDatabaseOpen()) return false;

    QSqlQuery query(m_database);  // Use m_database
    query.prepare("DELETE FROM photos WHERE id = ? AND user_id = ?");
    query.addBindValue(photoId);
    query.addBindValue(userId);

    if (!query.exec()) {
        LOG_ERROR("Failed to remove photo: " + query.lastError().text().toStdString());
        return false;
    }

    return true;
}



// ==================== Query Operations ====================

std::optional<Core::Entities::User> UserDao::findUserByUsername(const QString& username) {
    if (!ensureDatabaseOpen()) return std::nullopt;

    QSqlQuery query(m_database);
    // Add avatar_uri to SELECT
    query.prepare("SELECT id, username, password, avatar_uri FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        Core::Entities::User user = extractUserFromQuery(query);

        QString avatarUri = query.value(3).toString();
        if (!avatarUri.isEmpty()) {
            user.setAvatarUri(avatarUri);
        }

        return user;
    }

    return std::nullopt;
}

std::optional<Core::Entities::User> UserDao::findUserById(int userId) {
    if (!ensureDatabaseOpen()) return std::nullopt;

    QSqlQuery query(m_database);
    query.prepare("SELECT id, username, password, avatar_uri FROM users WHERE id = :id");
    query.bindValue(":id", userId);

    if (query.exec() && query.next()) {
        Core::Entities::User user = extractUserFromQuery(query);

        QString avatarUri = query.value(3).toString();
        if (!avatarUri.isEmpty()) {
            user.setAvatarUri(avatarUri);
        }

        return user;
    }

    return std::nullopt;
}

std::vector<Core::Entities::User> UserDao::loadAllUsers() {
    std::vector<Core::Entities::User> users;
    if (!ensureDatabaseOpen()) return users;

    QSqlQuery query(m_database);

    // Add avatar_uri to SELECT - THIS WAS MISSING
    if (!query.exec("SELECT id, username, password, avatar_uri FROM users ORDER BY id")) {
        LOG_ERROR("Query failed: " + query.lastError().text().toStdString());
        return users;
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        QString username = query.value(1).toString();
        QString encryptedPasswordHash = query.value(2).toString();
        QString avatarUri = query.value(3).toString();  // Get avatar_uri

        LOG_DEBUG("  ID: " + std::to_string(id) +
                  ", Username: " + username.toStdString() +
                  ", Avatar URI: " + avatarUri.toStdString());

        // Decrypt the password hash
        QString decryptedHash = decryptPassword(encryptedPasswordHash);

        // Create user
        Core::Entities::User user = Core::Entities::User::createExistingUser(id, username, decryptedHash);

        // Set avatar if exists
        if (!avatarUri.isEmpty()) {
            Core::Entities::Photo avatar;
            avatar.setUri(avatarUri);
            user.setAvatarUri(avatar.getFullPath());
        }

        users.push_back(user);
    }

    LOG_INFO("Loaded " + std::to_string(users.size()) + " users from database");
    return users;
}
bool UserDao::userExists(const QString& username) {
    if (!ensureDatabaseOpen()) return false;

     QSqlQuery query(m_database);
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        LOG_DEBUG("User exists check for '" + username.toStdString() + "': " + std::to_string(count));
        return count > 0;
    }

    return false;
}

bool UserDao::validateUserCredentials(const QString& username, const QString& password) {
    if (!ensureDatabaseOpen()) return false;

     QSqlQuery query(m_database);
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        QString encryptedStoredHash = query.value(0).toString();
        LOG_DEBUG("Found user, encrypted hash length: " + std::to_string(encryptedStoredHash.length()));

        // Decrypt the stored password hash
        QString storedHash = decryptPassword(encryptedStoredHash);
        LOG_DEBUG("Decrypted hash length: " + std::to_string(storedHash.length()));

        // Hash the input password
        QString inputHash = hashPassword(password);
        LOG_DEBUG("Input hash length: " + std::to_string(inputHash.length()));

        // Compare
        bool valid = (storedHash == inputHash);
        LOG_DEBUG("Credentials valid: " + std::string(valid ? "true" : "false"));

        return valid;
    }

    LOG_WARN("User not found: " + username.toStdString());
    return false;
}

// =============== JSON ===============//
bool UserDao::exportToJson(const QString& filePath) {
    auto users = loadAllUsers();

    QJsonArray usersArray;
    for (const auto& user : users) {
        QJsonObject userObj;
        userObj["id"] = user.getId();
        userObj["username"] = user.getUsername();
        userObj["password_hash"] = user.getPasswordHash();
        userObj["avatar_uri"] = user.getAvatar().getUri();  // Store image URI

        usersArray.append(userObj);
    }

    QJsonDocument doc(usersArray);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR("Cannot open file for JSON export");
        return false;
    }

    file.write(doc.toJson());
    file.close();

    LOG_INFO("Exported " + std::to_string(users.size()) + " users to " + filePath.toStdString());
    return true;
}

bool UserDao::importFromJson(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_WARN("JSON file not found: " + filePath.toStdString());
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        LOG_ERROR("Invalid JSON format");
        return false;
    }

    QJsonArray usersArray = doc.array();
    int imported = 0;

    for (const auto& item : usersArray) {
        QJsonObject obj = item.toObject();

        Core::Entities::User user;
        user.setUsername(obj["username"].toString());
        user.setPasswordHash(obj["password_hash"].toString());

        QString avatarUri = obj["avatar_uri"].toString();
        if (!avatarUri.isEmpty()) {
            Core::Entities::Photo avatar;
            avatar.setUri(avatarUri);
            user.setAvatarUri(avatar.getFullPath());
        }

        if (saveUser(user)) {
            imported++;
        }
    }

    LOG_INFO("Imported " + std::to_string(imported) + " users from JSON");
    return true;
}
bool UserDao::clearAllUsers() {
    if (!ensureDatabaseOpen()) return false;

    QSqlQuery query(m_database);

    // Disable foreign key checks temporarily
    query.exec("PRAGMA foreign_keys = OFF");

    // Delete all users (photos will be deleted automatically due to CASCADE)
    if (!query.exec("DELETE FROM users")) {
        LOG_ERROR("Failed to clear users: " + query.lastError().text().toStdString());
        query.exec("PRAGMA foreign_keys = ON");
        return false;
    }

    // Reset auto increment
    query.exec("DELETE FROM sqlite_sequence WHERE name='users'");

    // Re-enable foreign key checks
    query.exec("PRAGMA foreign_keys = ON");

    LOG_INFO("All users cleared from database");
    return true;
}



// ==================== Helper Methods ====================

Core::Entities::User UserDao::extractUserFromQuery(QSqlQuery& query) {
    int id = query.value(0).toInt();
    QString username = query.value(1).toString();
    QString encryptedPasswordHash = query.value(2).toString();

    LOG_DEBUG("Extracting user - ID: " + std::to_string(id) +
              ", Username: " + username.toStdString());
    LOG_DEBUG("Encrypted hash length: " + std::to_string(encryptedPasswordHash.length()));

    // Decrypt the password hash
    QString decryptedHash = decryptPassword(encryptedPasswordHash);
    LOG_DEBUG("Decrypted hash length: " + std::to_string(decryptedHash.length()));

    // Create user with decrypted hash
    return Core::Entities::User::createExistingUser(id, username, decryptedHash);
}





