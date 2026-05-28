//
// Created by lv on 4/28/26.
//

#include "Photo.h"
#include <QFileInfo>
#include <QDir>
#include <QBuffer>
#include <QImageReader>

namespace Core::Entities {

// Constructors
Photo::Photo()
    : m_id(-1)
    , m_userId(-1)
    , m_fileSize(0)
    , m_isAvatar(false) {
}

Photo::Photo(qint64 id, const QString& uri)
    : m_id(id)
    , m_userId(-1)
    , m_uri(uri)
    , m_fileSize(0)
    , m_isAvatar(false) {
}

Photo::Photo(const Photo& other) = default;

Photo& Photo::operator=(const Photo& other) = default;

// Factory methods
Photo Photo::createNew(qint64 id, const QString& uri, const QString& originalName) {
    Photo photo(id, uri);
    photo.m_originalName = originalName;
    photo.m_uploadTime = QDateTime::currentDateTime();
    photo.m_lastModified = QDateTime::currentDateTime();
    return photo;
}

Photo Photo::createAvatar(qint64 id, const QString& uri, const QString& originalName) {
    Photo photo = createNew(id, uri, originalName);
    photo.m_isAvatar = true;
    return photo;
}

// Getters implementation
QString Photo::getDisplayName() const {
    if (!m_displayName.isEmpty()) {
        return m_displayName;
    }
    if (!m_originalName.isEmpty()) {
        return m_originalName;
    }
    return QString("Photo_%1").arg(m_id);
}

QString Photo::getFullPath() const {
    // If URI is already absolute, return it
    QFileInfo fileInfo(m_uri);
    if (fileInfo.isAbsolute()) {
        return m_uri;
    }
    // Otherwise, resolve relative path from current directory
    return QDir::current().absoluteFilePath(m_uri);
}

// Business logic
bool Photo::isValid() const {
    return m_id != -1 && !m_uri.isEmpty() && validateFileSize();
}

bool Photo::isImage() const {
    QStringList imageFormats = {"jpg", "jpeg", "png", "gif", "bmp", "webp"};
    QString ext = getFileExtension().toLower();
    return imageFormats.contains(ext);
}

bool Photo::validateMimeType() const {
    QStringList validMimeTypes = {
        "image/jpeg", "image/png", "image/gif",
        "image/bmp", "image/webp", "image/jpg"
    };
    return validMimeTypes.contains(m_mimeType);
}

void Photo::updateMetadata(const QString& mimeType, qint64 fileSize) {
    m_mimeType = mimeType;
    m_fileSize = fileSize;
    m_lastModified = QDateTime::currentDateTime();
}

QString Photo::getFileExtension() const {
    QFileInfo fileInfo(m_uri);
    return fileInfo.suffix();
}

void Photo::validatePhoto() {
    // Add validation logic if needed
}

} // namespace Core::Entities