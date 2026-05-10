//
// Created by lv on 5/5/26.
//

#include "PhotoPickerScreen.h"
#include "thumbnail_widget/PhotoPickerThumbnailWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QBuffer>
#include <QScrollArea>
#include <QFileInfo>
#include <QLabel>
#include <QPushButton>
#include "../../../logger/logger.h"
#include "core/data/local_db/dao/UserDao.h"
#include "data/PhotoStorageHandler.h"
#include <QTimer>

PhotoPickerScreen::PhotoPickerScreen(QWidget* parent)
    : QMainWindow(parent), m_selectedPhotoId(-1) {
    setupUI();
}

void PhotoPickerScreen::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Title
    m_titleLabel = new QLabel("My Photos", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);

    // Photo counter
    m_photoCountLabel = new QLabel(this);
    m_photoCountLabel->setAlignment(Qt::AlignCenter);

    // Upload button
    m_uploadBtn = new QPushButton("📸 Upload New Photo", this);
    m_uploadBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 10px; font-size: 14px; border-radius: 5px; }");

    // Photo grid scroll area
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setMinimumHeight(400);
    m_scrollArea->setStyleSheet("QScrollArea { border: 1px solid #ddd; border-radius: 5px; background-color: #f5f5f5; }");

    m_photoGridWidget = new QWidget();
    m_photoGridLayout = new QGridLayout(m_photoGridWidget);
    m_photoGridLayout->setSpacing(15);
    m_photoGridLayout->setContentsMargins(10, 10, 10, 10);
    m_scrollArea->setWidget(m_photoGridWidget);

    // Status label
    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setWordWrap(true);

    // Bottom buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    m_refreshBtn = new QPushButton("🔄 Refresh", this);
    m_backBtn = new QPushButton("← Back to Profile", this);

    m_refreshBtn->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 8px; border-radius: 4px; }");
    m_backBtn->setStyleSheet("QPushButton { background-color: #9E9E9E; color: white; padding: 8px; border-radius: 4px; }");

    buttonLayout->addWidget(m_refreshBtn);
    buttonLayout->addWidget(m_backBtn);

    // Add to main layout
    mainLayout->addWidget(m_titleLabel);
    mainLayout->addWidget(m_photoCountLabel);
    mainLayout->addWidget(m_uploadBtn);
    mainLayout->addWidget(m_scrollArea);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addLayout(buttonLayout);

    setCentralWidget(centralWidget);
    setWindowTitle("Photo Manager");
    resize(650, 550);

    // Connect signals
    connect(m_uploadBtn, &QPushButton::clicked, this, &PhotoPickerScreen::onUploadPhoto);
    connect(m_backBtn, &QPushButton::clicked, this, &PhotoPickerScreen::onBackClicked);
    connect(m_refreshBtn, &QPushButton::clicked, this, &PhotoPickerScreen::onRefresh);
}

void PhotoPickerScreen::setUser(const Core::Entities::User& user) {
    m_currentUser = user;
    loadPhotos();
}

void PhotoPickerScreen::loadPhotos() {
    // Clear existing thumbnails
    for (auto* widget : m_thumbnailWidgets) {
        delete widget;
    }
    m_thumbnailWidgets.clear();

    // Clear layout
    QLayoutItem* item;
    while ((item = m_photoGridLayout->takeAt(0)) != nullptr) {
        delete item;
    }

    // Load photos from storage
    m_userPhotos = PhotoStorageHandler::instance().getUserPhotos(m_currentUser.getId());

    // Check database for avatar status
    UserDao userDao;
    auto userFromDb = userDao.findUserById(m_currentUser.getId());
    if (userFromDb.has_value()) {
        Core::Entities::Photo avatarPhoto = userFromDb->getAvatar();
        QString avatarUri = avatarPhoto.getUri();

        for (auto& photo : m_userPhotos) {
            photo.setAsAvatar(!avatarUri.isEmpty() && photo.getUri() == avatarUri);
        }
    }

    // Add photo thumbnails to grid
    int row = 0, col = 0;
    int maxCols = 3;

    for (size_t i = 0; i < m_userPhotos.size(); ++i) {
        PhotoThumbnailWidget* thumbnail = new PhotoThumbnailWidget(m_userPhotos[i], this);
        thumbnail->setAsAvatar(m_userPhotos[i].isAvatar());

        connect(thumbnail, &PhotoThumbnailWidget::clicked, this, &PhotoPickerScreen::onPhotoSelected);
        connect(thumbnail, &PhotoThumbnailWidget::deleteRequested, this, &PhotoPickerScreen::onDeletePhoto);
        connect(thumbnail, &PhotoThumbnailWidget::setAsAvatarRequested, this, &PhotoPickerScreen::onSetAsAvatar);

        m_thumbnailWidgets.push_back(thumbnail);

        m_photoGridLayout->addWidget(thumbnail, row, col);
        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }


    m_photoCountLabel->setText(QString("📷 Photos: %1/4").arg(static_cast<int>(m_userPhotos.size())));
    updatePhotoLimitWarning();

    LOG_INFO("Loaded " + std::to_string(m_userPhotos.size()) + " photos for user " +
             m_currentUser.getUsername().toStdString());
}

void PhotoPickerScreen::onUploadPhoto() {
    if (m_userPhotos.size() >= 4) {
        showStatusMessage("❌ Maximum 4 photos allowed! Delete some photos first.", true);
        return;
    }

    QString filePath = QFileDialog::getOpenFileName(this,
        "Select Photo",
        QString(),
        "Images (*.png *.jpg *.jpeg *.bmp *.gif *.webp)");

    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        showStatusMessage("❌ Cannot open file", true);
        return;
    }

    QByteArray imageData = file.readAll();
    file.close();

    // Save photo
    Core::Entities::Photo newPhoto = PhotoStorageHandler::instance().savePhoto(
        m_currentUser.getId(),
        imageData,
        QFileInfo(filePath).baseName()
    );

    if (newPhoto.getId() != -1) {

        UserDao userDao;
        if (userDao.addPhotoToUser(m_currentUser.getId(), newPhoto)) {
            showStatusMessage("✅ Photo uploaded successfully!");
            onRefresh();
        }

    }

}

void PhotoPickerScreen::onPhotoSelected(const Core::Entities::Photo& photo) {
    clearSelection();
    m_selectedPhotoId = photo.getId();

    // Highlight selected thumbnail
    for (auto* thumbnail : m_thumbnailWidgets) {
        if (thumbnail->getPhoto().getId() == photo.getId()) {
            thumbnail->setSelected(true);
            break;
        }
    }

    showStatusMessage("Selected: " + photo.getDisplayName() + " — Click 'Set as Avatar' to make this your profile picture");
}

void PhotoPickerScreen::onDeletePhoto(const Core::Entities::Photo& photo) {
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        "Delete Photo",
        QString("Delete '%1'?\nThis action cannot be undone!").arg(photo.getDisplayName()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    // Delete from storage
    if (!PhotoStorageHandler::instance().deletePhoto(m_currentUser.getId(), photo)) {
        // showStatusMessage("❌ Failed to delete photo file", true);
        return;
    }

    // Delete from database
    UserDao userDao;
    if (!userDao.removePhotoFromUser(m_currentUser.getId(), photo.getId())) {
        // showStatusMessage("❌ Failed to delete photo metadata", true);
        return;
    }

    showStatusMessage("✅ Photo deleted!");

    // If this was the avatar, notify profile screen to update
    if (photo.isAvatar()) {
        emit avatarUpdated();
    }

    onRefresh();
}

void PhotoPickerScreen::onSetAsAvatar(const Core::Entities::Photo& photo) {

    UserDao userDao;

    // Update database with new avatar
    if (userDao.updateUserAvatar(m_currentUser.getId(), photo.getUri())) {
        showStatusMessage("✅ Avatar updated successfully!");
        emit avatarUpdated();
        onRefresh(); // Reload to update avatar badges
    } else {
        showStatusMessage("❌ Failed to set as avatar", true);
        LOG_ERROR("Failed to update avatar for user: " + std::to_string(m_currentUser.getId()));
    }
}

void PhotoPickerScreen::onBackClicked() {
    emit backToProfile();
    this->close();
}

void PhotoPickerScreen::onRefresh() {
    loadPhotos();
    showStatusMessage("✅ Refreshed!");
}

void PhotoPickerScreen::showStatusMessage(const QString& message, bool isError) {
    m_statusLabel->setText(message);
    m_statusLabel->setStyleSheet(isError ? "color: red; font-weight: bold;" : "color: green; font-weight: bold;");
    QTimer::singleShot(3000, [this]() {
        m_statusLabel->clear();
    });
}

void PhotoPickerScreen::clearSelection() {
    m_selectedPhotoId = -1;
    for (auto* thumbnail : m_thumbnailWidgets) {
        thumbnail->setSelected(false);
    }
}

void PhotoPickerScreen::updatePhotoLimitWarning() {
    if (m_userPhotos.size() >= 4) {
        m_uploadBtn->setEnabled(false);
        m_uploadBtn->setToolTip("Maximum 4 photos reached");
    } else {
        m_uploadBtn->setEnabled(true);
        m_uploadBtn->setToolTip("");
    }
}