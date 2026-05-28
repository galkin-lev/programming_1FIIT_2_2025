//
// Created by lv on 5/5/26.
//

#include "PhotoPickerThumbnailWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QFileInfo>
#include <QMouseEvent>
#include <QPainter>
#include "../../../logger/logger.h"

PhotoThumbnailWidget::PhotoThumbnailWidget(const Core::Entities::Photo& photo, QWidget* parent)
    : QWidget(parent)
    , m_photo(photo)
    , m_isSelected(false)
    , m_isHovered(false) {
    setupUI();
    updateStyle();
}

void PhotoThumbnailWidget::setupUI() {
    setFixedSize(140, 170);
    setCursor(Qt::PointingHandCursor);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(5);

    // Thumbnail container
    QWidget* thumbnailContainer = new QWidget(this);
    thumbnailContainer->setFixedSize(120, 120);
    thumbnailContainer->setStyleSheet("background-color: white; border-radius: 8px;");

    QVBoxLayout* thumbLayout = new QVBoxLayout(thumbnailContainer);
    thumbLayout->setContentsMargins(0, 0, 0, 0);

    // Thumbnail label
    m_thumbnailLabel = new QLabel(thumbnailContainer);
    m_thumbnailLabel->setAlignment(Qt::AlignCenter);
    m_thumbnailLabel->setFixedSize(120, 120);
    m_thumbnailLabel->setStyleSheet("QLabel { background-color: transparent; border-radius: 8px; }");

    // Load and scale thumbnail
    QPixmap original(m_photo.getFullPath());
    if (!original.isNull()) {
        QPixmap scaled = original.scaled(110, 110, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_thumbnailLabel->setPixmap(scaled);
        m_thumbnailLabel->setToolTip(m_photo.getDisplayName());
    } else {
        // Placeholder for missing image
        m_thumbnailLabel->setText("📷\nNo Image");
        m_thumbnailLabel->setStyleSheet("font-size: 20px; color: #999;");
        LOG_WARN("Failed to load thumbnail: " + m_photo.getFullPath().toStdString());
    }

    thumbLayout->addWidget(m_thumbnailLabel, 0, Qt::AlignCenter);

    // Selection overlay
    m_selectionOverlay = new QWidget(thumbnailContainer);
    m_selectionOverlay->setGeometry(0, 0, 120, 120);
    m_selectionOverlay->setStyleSheet("background-color: rgba(33, 150, 243, 0.4); border-radius: 8px;");
    m_selectionOverlay->hide();

    // Name label (truncated)
    QString displayName = m_photo.getDisplayName();
    if (displayName.length() > 15) {
        displayName = displayName.left(12) + "...";
    }
    m_nameLabel = new QLabel(displayName, this);
    m_nameLabel->setAlignment(Qt::AlignCenter);
    m_nameLabel->setStyleSheet("font-size: 10px; font-weight: bold; color: #333;");
    m_nameLabel->setToolTip(m_photo.getDisplayName());

    // Size label
    double sizeKB = m_photo.getFileSize() / 1024.0;
    QString sizeText;
    if (sizeKB > 1024) {
        sizeText = QString::number(sizeKB / 1024.0, 'f', 1) + " MB";
    } else {
        sizeText = QString::number(sizeKB, 'f', 0) + " KB";
    }
    m_sizeLabel = new QLabel(sizeText, this);
    m_sizeLabel->setAlignment(Qt::AlignCenter);
    m_sizeLabel->setStyleSheet("font-size: 8px; color: #666;");

    // Button layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(8);
    buttonLayout->setContentsMargins(5, 0, 5, 0);

    // Avatar button (star)
    m_avatarBtn = new QPushButton("⭐", this);
    m_avatarBtn->setFixedSize(30, 25);
    m_avatarBtn->setToolTip("Set as Profile Picture");
    m_avatarBtn->setCursor(Qt::PointingHandCursor);
    m_avatarBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #FF9800;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 4px;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #F57C00;"
        "}"
    );

    // Delete button (trash)
    m_deleteBtn = new QPushButton("🗑", this);
    m_deleteBtn->setFixedSize(30, 25);
    m_deleteBtn->setToolTip("Delete Photo");
    m_deleteBtn->setCursor(Qt::PointingHandCursor);
    m_deleteBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #f44336;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 4px;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #D32F2F;"
        "}"
    );

    buttonLayout->addWidget(m_avatarBtn);
    buttonLayout->addWidget(m_deleteBtn);

    // Add all to main layout
    mainLayout->addWidget(thumbnailContainer, 0, Qt::AlignCenter);
    mainLayout->addWidget(m_nameLabel);
    mainLayout->addWidget(m_sizeLabel);
    mainLayout->addLayout(buttonLayout);

    // Connect buttons
    connect(m_avatarBtn, &QPushButton::clicked, this, [this]() {
        emit setAsAvatarRequested(m_photo);
    });

    connect(m_deleteBtn, &QPushButton::clicked, this, [this]() {
        emit deleteRequested(m_photo);
    });
}

void PhotoThumbnailWidget::setSelected(bool selected) {
    m_isSelected = selected;
    if (selected) {
        m_selectionOverlay->show();
    } else {
        m_selectionOverlay->hide();
    }
    updateStyle();
}

void PhotoThumbnailWidget::setAsAvatar(bool isAvatar) {
    if (isAvatar) {
        m_avatarBtn->setText("👑");
        m_avatarBtn->setToolTip("Current Profile Picture");
        m_avatarBtn->setStyleSheet(
            "QPushButton {"
            "   background-color: #4CAF50;"
            "   color: white;"
            "   border: none;"
            "   border-radius: 4px;"
            "   font-size: 12px;"
            "}"
        );
        m_avatarBtn->setEnabled(false);

        // Add crown badge to thumbnail
        QLabel* crownBadge = new QLabel("👑", this);
        crownBadge->setGeometry(10, 10, 30, 30);
        crownBadge->setStyleSheet("font-size: 24px; background-color: transparent;");
        crownBadge->raise();
    } else {
        m_avatarBtn->setText("⭐");
        m_avatarBtn->setToolTip("Set as Profile Picture");
        m_avatarBtn->setStyleSheet(
            "QPushButton {"
            "   background-color: #FF9800;"
            "   color: white;"
            "   border: none;"
            "   border-radius: 4px;"
            "   font-size: 12px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #F57C00;"
            "}"
        );
        m_avatarBtn->setEnabled(true);
    }
    updateStyle();
}

void PhotoThumbnailWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(m_photo);
    }
    QWidget::mousePressEvent(event);
}

void PhotoThumbnailWidget::enterEvent(QEnterEvent* event) {
    m_isHovered = true;
    updateStyle();
    QWidget::enterEvent(event);
}

void PhotoThumbnailWidget::leaveEvent(QEvent* event) {
    m_isHovered = false;
    updateStyle();
    QWidget::leaveEvent(event);
}

void PhotoThumbnailWidget::updateStyle() {
    QString baseStyle;

    if (m_isSelected) {
        baseStyle = "QWidget { background-color: #E3F2FD; border: 2px solid #2196F3; border-radius: 10px; }";
    } else if (m_isHovered) {
        baseStyle = "QWidget { background-color: #F5F5F5; border: 2px solid #FF9800; border-radius: 10px; }";
    } else {
        baseStyle = "QWidget { background-color: #FAFAFA; border: 1px solid #E0E0E0; border-radius: 10px; }";
    }

    setStyleSheet(baseStyle);
}

// Helper to get file size string
static QString formatFileSize(qint64 bytes) {
    if (bytes < 1024) return QString::number(bytes) + " B";
    if (bytes < 1024 * 1024) return QString::number(bytes / 1024.0, 'f', 1) + " KB";
    if (bytes < 1024 * 1024 * 1024) return QString::number(bytes / (1024.0 * 1024.0), 'f', 1) + " MB";
    return QString::number(bytes / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
}