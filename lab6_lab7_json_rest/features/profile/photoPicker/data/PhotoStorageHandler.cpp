//
// Created by lv on 5/10/26.
//

#include "PhotoStorageHandler.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QUuid>
#include <QImageReader>
#include <QBuffer>
#include <QCryptographicHash>
#include <QThreadPool>
#include <algorithm>  // ADD THIS for std::sort
#include <vector>     // ADD THIS
#include "../../../../logger/logger.h"
#include <QtConcurrent>
#include <QFutureWatcher>

// Remove duplicate includes: QtConcurrent/QtConcurrent and second QThreadPool

PhotoStorageHandler::PhotoStorageHandler()
    : m_imageCache(MAX_CACHE_ITEMS) {
    m_imageCache.setMaxCost(MAX_CACHE_SIZE);
    LOG_DEBUG("PhotoStorageHandler initialized with cache size: " +
              std::to_string(MAX_CACHE_SIZE / (1024 * 1024)) + " MB");
}

PhotoStorageHandler::~PhotoStorageHandler() {
    clearCache();
}

PhotoStorageHandler& PhotoStorageHandler::instance() {
    static PhotoStorageHandler instance;
    return instance;
}

// ==================== FILE OPERATIONS ====================

QString PhotoStorageHandler::getBasePath() const {

    return QDir::current().absoluteFilePath("user_photos");
}

QString PhotoStorageHandler::getUserPhotoDirectory(int userId) {
    return QString("%1/user_%2").arg(getBasePath()).arg(userId);
}

bool PhotoStorageHandler::ensureUserDirectory(int userId) {
    QString userPath = getUserPhotoDirectory(userId);
    QDir dir;

    if (!dir.exists(userPath)) {
        if (!dir.mkpath(userPath)) {
            LOG_ERROR("Failed to create user photo directory: " + userPath.toStdString());
            return false;
        }
        LOG_DEBUG("Created user photo directory: " + userPath.toStdString());
    }

    return true;
}

QString PhotoStorageHandler::generateUniqueFilename(int userId, const QString& extension) {
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces).left(8);

    // Hash to ensure uniqueness
    QString uniqueStr = QString("%1_%2_%3").arg(userId).arg(timestamp).arg(uuid);
    QByteArray hash = QCryptographicHash::hash(uniqueStr.toUtf8(), QCryptographicHash::Md5);
    QString hashStr = hash.toHex().left(8);

    return QString("user_%1_%2_%3.%4")
        .arg(userId)
        .arg(timestamp)
        .arg(hashStr)
        .arg(extension);
}

QString PhotoStorageHandler::savePhotoToDisk(int userId, const QByteArray& imageData, const QString& originalName) {
    // Validate image
    if (!isValidImage(imageData)) {
        LOG_ERROR("Invalid image data for user " + std::to_string(userId));
        return QString();
    }

    // Ensure user directory exists
    if (!ensureUserDirectory(userId)) {
        return QString();
    }

    // Get image format
    QBuffer buffer(const_cast<QByteArray*>(&imageData));
    buffer.open(QIODevice::ReadOnly);
    QImageReader reader(&buffer);
    QString format = reader.format().toLower();
    if (format.isEmpty()) format = "jpg";
    if (format == "jpeg") format = "jpg";

    // Generate unique filename
    QString filename = generateUniqueFilename(userId, format);
    QString fullPath = getUserPhotoDirectory(userId) + "/" + filename;

    // Save file
    QFile file(fullPath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR("Failed to save photo: " + fullPath.toStdString());
        return QString();
    }

    file.write(imageData);
    file.close();

    LOG_INFO("Photo saved to disk: " + fullPath.toStdString());
    return fullPath;
}

bool PhotoStorageHandler::deletePhotoFromDisk(const QString& filePath) {
    QFile file(filePath);

    if (!file.exists()) {
        LOG_WARN("Photo file doesn't exist: " + filePath.toStdString());
        return false;
    }

    if (!file.remove()) {
        LOG_ERROR("Failed to delete photo file: " + filePath.toStdString());
        return false;
    }

    // Remove from cache if present
    m_imageCache.remove(filePath);

    LOG_INFO("Photo deleted from disk: " + filePath.toStdString());
    return true;
}

QStringList PhotoStorageHandler::getUserPhotoFiles(int userId) {
    QStringList files;
    QString userPath = getUserPhotoDirectory(userId);
    QDir dir(userPath);

    if (!dir.exists()) {
        return files;
    }

    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.gif" << "*.webp";

    QFileInfoList entries = dir.entryInfoList(filters, QDir::Files, QDir::Name);

    for (const QFileInfo& entry : entries) {
        files.append(entry.absoluteFilePath());
    }

    return files;
}

bool PhotoStorageHandler::userDirectoryExists(int userId) {
    QDir dir(getUserPhotoDirectory(userId));
    return dir.exists();
}

// ==================== PHOTO MANAGEMENT ====================

std::vector<Core::Entities::Photo> PhotoStorageHandler::getUserPhotos(int userId) {
    std::vector<Core::Entities::Photo> photos;

    QStringList photoFiles = getUserPhotoFiles(userId);

    for (const QString& filePath : photoFiles) {
        Core::Entities::Photo photo;
        photo.setId(0); // Will be set from database if needed
        photo.setUserId(userId);
        photo.setUri(filePath);

        // Extract display name from filename
        QFileInfo fileInfo(filePath);
        QString displayName = fileInfo.baseName();

        QStringList parts = displayName.split('_');
        if (parts.size() >= 4) {
            // Format: user_{userId}_{timestamp}_{hash}
            displayName = "Photo_" + parts[2]; // Use timestamp part
        }
        photo.setDisplayName(displayName);

        QDateTime created = fileInfo.birthTime();
        if (!created.isValid()) {
            created = fileInfo.lastModified();
        }
        photo.setCreatedAt(created);

        photo.setAsAvatar(false);

        photos.push_back(photo);
    }

    std::sort(photos.begin(), photos.end(),
        [](const Core::Entities::Photo& a, const Core::Entities::Photo& b) {
            return a.getCreatedAt() > b.getCreatedAt();
        });

    LOG_INFO("Loaded " + std::to_string(photos.size()) + " photos for user " + std::to_string(userId));
    return photos;
}

Core::Entities::Photo PhotoStorageHandler::savePhoto(int userId, const QByteArray& imageData, const QString& fileName) {
    Core::Entities::Photo photo;

    // Save to disk using existing method
    QString fullPath = savePhotoToDisk(userId, imageData, fileName);

    if (fullPath.isEmpty()) {
        LOG_ERROR("Failed to save photo for user " + std::to_string(userId));
        return photo; // Return empty photo
    }

    // Create photo object
    photo.setId(0); // ID will be set when saving to database
    photo.setUserId(userId);
    photo.setUri(fullPath);
    photo.setCreatedAt(QDateTime::currentDateTime());

    // Set display name
    QFileInfo fileInfo(fileName);
    QString displayName = fileInfo.baseName();
    if (displayName.isEmpty()) {
        displayName = "Photo_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    }
    photo.setDisplayName(displayName);
    photo.setAsAvatar(false);

    LOG_INFO("Photo saved successfully for user " + std::to_string(userId));
    return photo;
}

bool PhotoStorageHandler::deletePhoto(int userId, const Core::Entities::Photo& photo) {
    QString filePath = photo.getUri();

    if (filePath.isEmpty()) {
        LOG_ERROR("Empty file path for photo deletion");
        return false;
    }

    // Delete from disk
    bool deleted = deletePhotoFromDisk(filePath);

    if (deleted) {
        LOG_INFO("Photo deleted successfully for user " + std::to_string(userId));
    } else {
        LOG_ERROR("Failed to delete photo for user " + std::to_string(userId));
    }

    return deleted;
}

bool PhotoStorageHandler::updatePhotoMetadata(int userId, const Core::Entities::Photo& photo) {
    // Implementation for updating photo metadata
    // You can store this in a database or a metadata file
    LOG_DEBUG("Updating photo metadata for user " + std::to_string(userId) +
              ", photo ID: " + std::to_string(photo.getId()));
    return true; // Placeholder
}

// ==================== IMAGE LOADING (Like Coil) ====================

QString PhotoStorageHandler::getCacheKey(const QString& filePath, int size) {
    return QString("%1_%2x%2").arg(filePath).arg(size);
}

QPixmap PhotoStorageHandler::loadAndScaleImage(const QString& filePath, int targetSize) {
    QImageReader reader(filePath);
    reader.setAutoTransform(true);

    if (targetSize > 0) {
        reader.setScaledSize(QSize(targetSize, targetSize));
    }

    QImage image = reader.read();

    if (image.isNull()) {
        LOG_WARN("Failed to load image: " + filePath.toStdString());
        return QPixmap();
    }

    return QPixmap::fromImage(image);
}

QPixmap PhotoStorageHandler::loadImage(const QString& filePath, int targetSize) {
    if (filePath.isEmpty() || !QFile::exists(filePath)) {
        LOG_WARN("File doesn't exist: " + filePath.toStdString());
        return QPixmap();
    }

    QString cacheKey = getCacheKey(filePath, targetSize);

    // Check cache
    if (m_imageCache.contains(cacheKey)) {
        QPixmap* cached = m_imageCache[cacheKey];
        if (cached && !cached->isNull()) {
            LOG_DEBUG("Cache hit for: " + filePath.toStdString());
            return *cached;
        }
    }

    // Load and scale image
    QPixmap pixmap = loadAndScaleImage(filePath, targetSize);

    if (!pixmap.isNull()) {
        // Store in cache (estimate cost as approx pixel memory)
        int cost = pixmap.width() * pixmap.height() * 4; // RGBA ~4 bytes per pixel
        m_imageCache.insert(cacheKey, new QPixmap(pixmap), cost);
        LOG_DEBUG("Cached image: " + filePath.toStdString() + " (cost: " + std::to_string(cost) + ")");
    }

    return pixmap;
}

QPixmap PhotoStorageHandler::loadThumbnail(const QString& filePath, int size) {
    // Thumbnails are always cached with consistent size
    return loadImage(filePath, size);
}

QFuture<QPixmap> PhotoStorageHandler::loadImageAsync(const QString& filePath, int targetSize) {
    QString cacheKey = getCacheKey(filePath, targetSize);

    // Check if already loading
    if (m_pendingLoads.contains(cacheKey)) {
        return m_pendingLoads[cacheKey];
    }

    // Start async loading
    QFuture<QPixmap> future = QtConcurrent::run([this, filePath, targetSize]() {
        return loadImage(filePath, targetSize);
    });

    m_pendingLoads[cacheKey] = future;

    // Remove from pending when done
    QFutureWatcher<QPixmap>* watcher = new QFutureWatcher<QPixmap>();
    watcher->setFuture(future);
    QObject::connect(watcher, &QFutureWatcher<QPixmap>::finished, [this, cacheKey, watcher]() {
        m_pendingLoads.remove(cacheKey);
        watcher->deleteLater();
    });

    return future;
}

void PhotoStorageHandler::preloadImages(const QStringList& filePaths, int targetSize) {
    LOG_DEBUG("Preloading " + std::to_string(filePaths.size()) + " images");

    for (const QString& path : filePaths) {
        if (!m_imageCache.contains(getCacheKey(path, targetSize))) {
            loadImageAsync(path, targetSize);
        }
    }
}

void PhotoStorageHandler::clearCache() {
    m_imageCache.clear();
    m_pendingLoads.clear();
    LOG_DEBUG("Image cache cleared");
}

void PhotoStorageHandler::clearUserCache(int userId) {
    QString userDir = getUserPhotoDirectory(userId);

    // Remove cached images for this user
    QStringList keysToRemove;
    for (const QString& key : m_imageCache.keys()) {
        if (key.startsWith(userDir)) {
            keysToRemove.append(key);
        }
    }

    for (const QString& key : keysToRemove) {
        m_imageCache.remove(key);
    }

    LOG_DEBUG("Cleared cache for user " + std::to_string(userId) +
              " (" + std::to_string(keysToRemove.size()) + " items)");
}

// ==================== UTILITIES ====================

QSize PhotoStorageHandler::getImageSize(const QString& filePath) {
    QImageReader reader(filePath);
    return reader.size();
}

bool PhotoStorageHandler::isValidImage(const QString& filePath) {
    QImageReader reader(filePath);
    return reader.canRead();
}

bool PhotoStorageHandler::isValidImage(const QByteArray& data) {
    QBuffer buffer(const_cast<QByteArray*>(&data));
    buffer.open(QIODevice::ReadOnly);
    QImageReader reader(&buffer);

    if (!reader.canRead()) {
        return false;
    }

    // Check supported formats
    QString format = reader.format().toLower();
    QStringList supportedFormats = {"jpg", "jpeg", "png", "bmp", "gif", "webp"};

    return supportedFormats.contains(format);
}

QString PhotoStorageHandler::getImageFormat(const QString& filePath) {
    QImageReader reader(filePath);
    return reader.format().toLower();
}