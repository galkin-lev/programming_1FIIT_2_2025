//
// Created by lv on 4/28/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_PHOTO_H
#define LAB6_LOGIN_REGISTRATION_PHOTO_H

#include <QString>
#include <QDateTime>


#include <QString>
#include <QDateTime>
#include <QMetaType>
#include <memory>

namespace Core::Entities {

class Photo {
public:
    Photo();
    Photo(qint64 id, const QString& uri);
    Photo(const Photo& other);
    Photo& operator=(const Photo& other);

    // Factory methods (for creating valid photos)
    static Photo createNew(qint64 id, const QString& uri, const QString& originalName);
    static Photo createAvatar(qint64 id, const QString& uri, const QString& originalName);

    // Getters - business data
    qint64 getId() const { return m_id; }
    int getUserId() const { return m_userId; }  // ADD THIS
    QString getUri() const { return m_uri; }
    QString getOriginalName() const { return m_originalName; }
    QString getDisplayName() const;  // You have this declared, need implementation
    QString getMimeType() const { return m_mimeType; }
    qint64 getFileSize() const { return m_fileSize; }
    QDateTime getUploadTime() const { return m_uploadTime; }
    QDateTime getCreatedAt() const { return m_uploadTime; }  // ADD THIS (alias for upload time)
    QDateTime getLastModified() const { return m_lastModified; }

    // Business logic getters
    bool isAvatar() const { return m_isAvatar; }
    bool isValid() const;
    bool isImage() const;
    bool isTooLarge() const { return m_fileSize > MAX_FILE_SIZE; }

    // Get full absolute path (resolves relative paths)
    QString getFullPath() const;

    // Setters (for loading from database or storage handler)
    void setId(qint64 id) { m_id = id; }  // ADD THIS
    void setUserId(int userId) { m_userId = userId; }  // ADD THIS
    void setUri(const QString& uri) { m_uri = uri; }  // ADD THIS
    void setDisplayName(const QString& displayName) { m_displayName = displayName; }  // ADD THIS
    void setCreatedAt(const QDateTime& time) { m_uploadTime = time; }  // ADD THIS
    void setMimeType(const QString& mimeType) { m_mimeType = mimeType; }
    void setFileSize(qint64 fileSize) { m_fileSize = fileSize; }
    void setUploadTime(const QDateTime& time) { m_uploadTime = time; }
    void setAsAvatar(bool isAvatar) { m_isAvatar = isAvatar; }  // You have this through inline

    // Business logic methods
    void updateMetadata(const QString& mimeType, qint64 fileSize);
    QString getFileExtension() const;

    // Business rules (constants)
    static constexpr qint64 MAX_FILE_SIZE = 10 * 1024 * 1024;  // 10 MB
    static constexpr int MAX_AVATAR_DIMENSION = 500;            // 500px
    static constexpr int THUMBNAIL_SIZE = 150;                  // 150px

    // Domain validation
    bool validateFileSize() const { return m_fileSize <= MAX_FILE_SIZE; }
    bool validateMimeType() const;

    // Operators
    bool operator==(const Photo& other) const { return m_id == other.m_id; }
    bool operator!=(const Photo& other) const { return !(*this == other); }

private:
    void validatePhoto();

    qint64 m_id = -1;
    int m_userId = -1;  // ADD THIS - user ID who owns the photo
    QString m_uri;           // Can be relative or absolute path
    QString m_originalName;
    QString m_displayName;   // ADD THIS - for UI display
    QString m_mimeType;
    qint64 m_fileSize = 0;
    QDateTime m_uploadTime;
    QDateTime m_lastModified;
    bool m_isAvatar = false;
};

} // namespace Core::Entities

#endif //LAB6_LOGIN_REGISTRATION_PHOTO_H
