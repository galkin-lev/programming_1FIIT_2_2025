//
// Created by lv on 2/24/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_USER_H
#define LAB6_LOGIN_REGISTRATION_USER_H


#include <string>
#include <QString>
#include <QList>
#include <QMetaType>
#include "Photo.h"

namespace Core::Entities {

/**
 * User entity - represents an application user
 * Contains all business rules for user management
 */
class User {
public:
    // Constructors
    User();
    User(int id, const QString& username);
    User(const User& other);
    User& operator=(const User& other);

    // Factory methods
    static User createNewUser(const QString& username, const QString& password);
    static User createExistingUser(int id, const QString& username, const QString& passwordHash);

    // Getters
    int getId() const { return m_id; }
    QString getUsername() const { return m_username; }
    QString getPasswordHash() const { return m_passwordHash; }
    QDateTime getCreatedAt() const { return m_createdAt; }
    QDateTime getLastLogin() const { return m_lastLogin; }

    // Photo management
    QList<Photo> getPhotos() const { return m_photos; }
    int getPhotoCount() const { return m_photos.size(); }

    // Business rules (domain logic)
    bool canAddPhoto() const { return m_photos.size() < MAX_PHOTOS_PER_USER; }
    bool hasAvatar() const { return !getAvatar().getUri().isEmpty(); }
    bool isNewUser() const { return m_id == -1; }
    bool isPasswordValid(const QString& plainPassword) const;
    bool hasPhoto(qint64 photoId) const;
    bool hasPhoto(const QString& uri) const;


    // Business operations (mutating)
    void setUsername(const QString& username);
    void setPasswordHash(const QString& hash) { m_passwordHash = hash; }
    void updatePassword(const QString& newPassword);
    void addPhoto(const Photo& photo);
    bool removePhoto(qint64 photoId);
    bool removePhoto(const QString& uri);
    void setAvatarUri(const QString& uri) {
        if (!uri.isEmpty()) {
            m_avatar.setUri(uri);
        }
    }
    Photo getAvatar() const { return m_avatar; }   void recordLogin();



    // Domain validation
    bool validateUsername() const;
    bool validatePasswordStrength(const QString& password) const;

    // Constants (business rules)
    static constexpr int MAX_PHOTOS_PER_USER = 4;
    static constexpr int MIN_USERNAME_LENGTH = 3;
    static constexpr int MAX_USERNAME_LENGTH = 50;
    static constexpr int MIN_PASSWORD_LENGTH = 6;

    // Operators
    bool operator==(const User& other) const { return m_id == other.m_id; }
    bool operator!=(const User& other) const { return !(*this == other); }





private:
    void validateUser();

    int m_id = -1;
    QString m_username;
    QString m_passwordHash;
    QList<Photo> m_photos;
    QDateTime m_createdAt;
    QDateTime m_lastLogin;
    Photo m_avatar;


};

} // namespace Core::Entities

// Q_DECLARE_METATYPE(Core::Entities::User)








// using namespace std;
//
// struct User {
//
//     int id = -1;
//     string username;
//     string password;
//
// };


#endif //LAB6_LOGIN_REGISTRATION_USER_H