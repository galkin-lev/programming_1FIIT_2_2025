//
// Created by lv on 4/28/26.
//

// core/entities/User.cpp
#include "User.h"
#include <QCryptographicHash>
#include <QRegularExpression>



// core/entities/User.cpp
#include <QCryptographicHash>
#include <QRegularExpression>

#include "exceptions/DomainException.h"
#include "../../logger/logger.h"

namespace Core::Entities {

User::User() = default;

User::User(int id, const QString& username)
    : m_id(id), m_username(username) {
    validateUser();
}

User::User(const User& other) = default;

User& User::operator=(const User& other) = default;

User User::createNewUser(const QString& username, const QString& password) {
    User user(-1, username);

    // Hash the password
    QByteArray hashed = QCryptographicHash::hash(
        password.toUtf8(),
        QCryptographicHash::Sha256
    );
    user.m_passwordHash = QString(hashed.toHex());
    user.m_createdAt = QDateTime::currentDateTime();

    user.validateUser();
    return user;
}

User User::createExistingUser(int id, const QString& username, const QString& passwordHash) {
    User user(id, username);
    user.m_passwordHash = passwordHash;
    user.validateUser();
    return user;
}

bool User::isPasswordValid(const QString& plainPassword) const {
    QByteArray hashed = QCryptographicHash::hash(
        plainPassword.toUtf8(),
        QCryptographicHash::Sha256
    );
    return m_passwordHash == QString(hashed.toHex());
}

void User::setUsername(const QString& username) {
    m_username = username;
    validateUser();
}

void User::updatePassword(const QString& newPassword) {
    if (!validatePasswordStrength(newPassword)) {
        throw EntityValidationException("User", "password", "Password too weak");
    }

    QByteArray hashed = QCryptographicHash::hash(
        newPassword.toUtf8(),
        QCryptographicHash::Sha256
    );
    m_passwordHash = QString(hashed.toHex());
}

void User::recordLogin() {
    m_lastLogin = QDateTime::currentDateTime();
}

bool User::validateUsername() const {
    if (m_username.length() < MIN_USERNAME_LENGTH ||
        m_username.length() > MAX_USERNAME_LENGTH) {
        return false;
    }

    // Username must contain only alphanumeric and underscore
    QRegularExpression regex("^[a-zA-Z0-9_]+$");
    return regex.match(m_username).hasMatch();
}

bool User::validatePasswordStrength(const QString& password) const {
    if (password.length() < MIN_PASSWORD_LENGTH) {
        return false;
    }

    // At least one number and one letter
    bool hasDigit = false;
    bool hasLetter = false;

    for (QChar ch : password) {
        if (ch.isDigit()) hasDigit = true;
        if (ch.isLetter()) hasLetter = true;
    }

    return hasDigit && hasLetter;
}

void User::validateUser() {
    if (!validateUsername()) {
        // Log warning but don't throw in constructor
        // LOG_WARN("Invalid username:" + m_username.toStdString());
    }
}

    // ==================== Photo Management Implementation ====================
//
// Photo User::getAvatar() const {
//     for (const Photo& photo : m_photos) {
//         if (photo.isAvatar()) {
//             return photo;
//         }
//     }
//     return Photo();  // Return empty photo (no avatar)
// }

bool User::hasPhoto(qint64 photoId) const {
    for (const Photo& photo : m_photos) {
        if (photo.getId() == photoId) {
            return true;
        }
    }
    return false;
}

bool User::hasPhoto(const QString& uri) const {
    for (const Photo& photo : m_photos) {
        if (photo.getUri() == uri) {
            return true;
        }
    }
    return false;
}

void User::addPhoto(const Photo& photo) {
    // Check if we can add more photos
    if (!canAddPhoto()) {
        throw std::overflow_error("Cannot add more than " +
                                   std::to_string(MAX_PHOTOS_PER_USER) +
                                   " photos");
    }

    // Check if photo already exists
    if (hasPhoto(photo.getId())) {
        throw std::invalid_argument("Photo already exists in user's collection");
    }

    // Add the photo
    m_photos.append(photo);

    // Log for debugging (you can use your LOG_DEBUG macro)
    // LOG_DEBUG("Added photo " + QString::number(photo.getId()).toStdString() +
    //           " to user " + m_username.toStdString());
}

bool User::removePhoto(qint64 photoId) {
    // Find and remove the photo
    for (int i = 0; i < m_photos.size(); ++i) {
        if (m_photos[i].getId() == photoId) {
            // Check if this was the avatar before removing
            bool wasAvatar = m_photos[i].isAvatar();
            m_photos.removeAt(i);

            // If we removed the avatar, we need to clear avatar reference
            // (avatar will be set to empty photo)
            if (wasAvatar) {
                // Avatar is now gone - no need to do anything else
                // The getAvatar() will return empty photo now
            }

            return true;
        }
    }
    return false;
}

bool User::removePhoto(const QString& uri) {
    // Find and remove the photo by URI
    for (int i = 0; i < m_photos.size(); ++i) {
        if (m_photos[i].getUri() == uri) {
            m_photos.removeAt(i);
            return true;
        }
    }
    return false;
}
//
// void User::setAvatar(const Photo& avatar) {
//     // First, verify that this photo belongs to the user
//     bool photoOwned = false;
//     int photoIndex = -1;
//
//     for (int i = 0; i < m_photos.size(); ++i) {
//         if (m_photos[i].getId() == avatar.getId()) {
//             photoOwned = true;
//             photoIndex = i;
//             break;
//         }
//     }
//
//     if (!photoOwned) {
//         throw std::invalid_argument("Cannot set avatar - photo does not belong to user");
//     }
//
//     // Set all photos' isAvatar flag to false, then set the selected one to true
//     for (Photo& photo : m_photos) {
//         photo.setAsAvatar(false);
//     }
//
//     // Set the new avatar
//     m_photos[photoIndex].setAsAvatar(true);
// }


////////////////////// Photo management \\\\\\\\\\\\\\\\\\\\\\\\\\\\

} // namespace Core::Entities