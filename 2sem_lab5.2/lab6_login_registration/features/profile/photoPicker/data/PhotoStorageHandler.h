//
// Created by lv on 5/10/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_PHOTOSTORAGEHANDLER_H
#define LAB6_LOGIN_REGISTRATION_PHOTOSTORAGEHANDLER_H


#include <QString>
#include <QList>
#include <QPixmap>
#include <QCache>
#include <QHash>
#include <QFuture>
#include "core/model/Photo.h"

/**
 * PhotoStorageHandler - Like Coil for Qt
 * Handles file operations, image loading, caching, and thumbnails
 */
class PhotoStorageHandler {
public:
    static PhotoStorageHandler& instance();
    std::vector<Core::Entities::Photo> getUserPhotos(int userId);
    Core::Entities::Photo savePhoto(int userId, const QByteArray& imageData, const QString& fileName);
    bool deletePhoto(int userId, const Core::Entities::Photo& photo);


    // ========== FILE OPERATIONS ==========

    // Save photo to disk, returns file path (to store in DB)
    QString savePhotoToDisk(int userId, const QByteArray& imageData, const QString& originalName);

    // Delete photo file from disk
    bool deletePhotoFromDisk(const QString& filePath);

    // Get all photo file paths for a user
    QStringList getUserPhotoFiles(int userId);

    // Check if user directory exists
    bool userDirectoryExists(int userId);

    // Get user's photo directory path
    QString getUserPhotoDirectory(int userId);

    // ========== IMAGE LOADING (Like Coil) ==========

    // Load image from path with automatic caching
    QPixmap loadImage(const QString& filePath, int targetSize = 0);

    // Load thumbnail (always cached, smaller size)
    QPixmap loadThumbnail(const QString& filePath, int size = 150);

    // Load image asynchronously (non-blocking)
    QFuture<QPixmap> loadImageAsync(const QString& filePath, int targetSize = 0);

    // Preload images into cache (for smoother scrolling)
    void preloadImages(const QStringList& filePaths, int targetSize = 150);

    bool updatePhotoMetadata(int userId, const Core::Entities::Photo& photo);

    // Clear cache
    void clearCache();
    void clearUserCache(int userId);

    // ========== UTILITIES ==========

    // Get image dimensions without loading full image
    QSize getImageSize(const QString& filePath);

    // Validate if file is a valid image
    bool isValidImage(const QString& filePath);

    bool isValidImage(const QByteArray& data);

    // Get image format
    QString getImageFormat(const QString& filePath);

private:
    PhotoStorageHandler();
    ~PhotoStorageHandler();
    PhotoStorageHandler(const PhotoStorageHandler&) = delete;
    PhotoStorageHandler& operator=(const PhotoStorageHandler&) = delete;

    // Cache helpers
    QString getCacheKey(const QString& filePath, int size);
    QPixmap loadAndScaleImage(const QString& filePath, int targetSize);

    // File helpers
    bool ensureUserDirectory(int userId);
    QString generateUniqueFilename(int userId, const QString& extension);
    QString getBasePath() const;

    // Cache storage
    QCache<QString, QPixmap> m_imageCache;
    QHash<QString, QFuture<QPixmap>> m_pendingLoads;

    // Maximum cache size (100MB)
    static constexpr int MAX_CACHE_SIZE = 100 * 1024 * 1024; // 100 MB
    static constexpr int MAX_CACHE_ITEMS = 200;
};

#endif //LAB6_LOGIN_REGISTRATION_PHOTOSTORAGEHANDLER_H