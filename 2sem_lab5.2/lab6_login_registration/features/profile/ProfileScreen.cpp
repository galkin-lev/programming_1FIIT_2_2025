//
// Created by lv on 5/5/26.
//

#include "../profile/ProfileScreen.h"

#include <QDir>

#include "../../core/model/Session.h"
#include "core/data/local_db/dao/UserDao.h"
#include "features/profile/photoPicker/data/PhotoStorageHandler.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QFile>
#include "../../logger/logger.h"
#include "editProfile/EditProfileScreen.h"

ProfileScreen::ProfileScreen(QWidget* parent) : QMainWindow(parent) {
    setupUI();
}


void ProfileScreen::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Avatar (centered)
    QHBoxLayout* avatarLayout = new QHBoxLayout();
    avatarLayout->setAlignment(Qt::AlignCenter);

    avatarLabel = new QLabel(this);
    avatarLabel->setFixedSize(120, 120);
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setStyleSheet(
        "QLabel {"
        "  background-color: #e0e0e0;"
        "  border-radius: 60px;"
        "  border: 2px solid #4CAF50;"
        "}"
    );
    avatarLayout->addWidget(avatarLabel);
    mainLayout->addLayout(avatarLayout);

    mainLayout->addSpacing(10);

    // Welcome label
    welcomeLabel = new QLabel(this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = welcomeLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    welcomeLabel->setFont(titleFont);

    // User info labels
    userIdLabel = new QLabel(this);
    userIdLabel->setAlignment(Qt::AlignCenter);

    sessionIdLabel = new QLabel(this);
    sessionIdLabel->setAlignment(Qt::AlignCenter);

    sessionDurationLabel = new QLabel(this);
    sessionDurationLabel->setAlignment(Qt::AlignCenter);

    // Buttons
    editProfileBtn = new QPushButton("✏️ Edit Profile", this);
    refreshBtn = new QPushButton("🔄 Refresh Session", this);
    logoutBtn = new QPushButton("🚪 Logout", this);

    // JSON buttons
    QPushButton* exportBtn = new QPushButton("📁 Export to JSON", this);
    QPushButton* importBtn = new QPushButton("📂 Import from JSON", this);

    // Style JSON buttons
    exportBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #9C27B0;"
        "  color: white;"
        "  padding: 8px;"
        "  border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #7B1FA2;"
        "}"
    );

    importBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #FF9800;"
        "  color: white;"
        "  padding: 8px;"
        "  border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #F57C00;"
        "}"
    );

    // Button styles
    editProfileBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #2196F3;"
        "  color: white;"
        "  padding: 8px;"
        "  border-radius: 4px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #1976D2;"
        "}"
    );

    refreshBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #4CAF50;"
        "  color: white;"
        "  padding: 8px;"
        "  border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #45a049;"
        "}"
    );

    logoutBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #f44336;"
        "  color: white;"
        "  padding: 8px;"
        "  border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #D32F2F;"
        "}"
    );

    // Add to layout
    mainLayout->addWidget(welcomeLabel);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(userIdLabel);
    mainLayout->addWidget(sessionIdLabel);
    mainLayout->addWidget(sessionDurationLabel);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(editProfileBtn);
    mainLayout->addWidget(refreshBtn);
    mainLayout->addWidget(logoutBtn);

    // Add separator line before JSON buttons
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);
    mainLayout->addSpacing(10);

    // Add JSON buttons at the bottom
    mainLayout->addWidget(exportBtn);
    mainLayout->addWidget(importBtn);

    mainLayout->addStretch();

    setCentralWidget(centralWidget);
    setWindowTitle("User Profile");
    resize(400, 650);  // Increased height to accommodate new buttons

    // Connect signals
    connect(logoutBtn, &QPushButton::clicked, this, &ProfileScreen::onLogoutClicked);
    connect(refreshBtn, &QPushButton::clicked, this, &ProfileScreen::onRefreshClicked);
    connect(editProfileBtn, &QPushButton::clicked, this, &ProfileScreen::onEditProfileClicked);

    // Connect JSON buttons
    connect(exportBtn, &QPushButton::clicked, [this]() {
        UserDao userDao;
        if (userDao.exportToJson()) {
            QMessageBox::information(this, "Export", "Users exported to JSON");
        } else {
            QMessageBox::warning(this, "Export Failed", "Failed to export users");
        }
    });

    connect(importBtn, &QPushButton::clicked, [this]() {
        UserDao userDao;

        // Clear database and import from JSON
        if (userDao.clearAllUsers() && userDao.importFromJson()) {
            QMessageBox::information(this, "Import", "Database cleared and users imported from JSON");
            onRefreshClicked();  // Refresh to show imported data
        } else {
            QMessageBox::warning(this, "Import Failed", "Failed to import users");
        }
    });

}


void ProfileScreen::setUser(const Core::Entities::User& user) {
    m_currentUser = user;
    updateDisplay();
    updateAvatarDisplay();
}

void ProfileScreen::onEditProfileClicked() {
    EditProfileScreen* editScreen = new EditProfileScreen(this);
    editScreen->setUser(m_currentUser);
    editScreen->show();
}

void ProfileScreen::refreshAvatar() {
    // Reload user from database to get updated avatar
    UserDao userDao;
    auto userFromDb = userDao.findUserById(m_currentUser.getId());

    if (userFromDb.has_value()) {
        m_currentUser = userFromDb.value();
        updateAvatarDisplay();
    }
}

void ProfileScreen::updateAvatarDisplay() {
    Core::Entities::Photo avatar = m_currentUser.getAvatar();

    if (!avatar.getUri().isEmpty()) {
        // Get full path to avatar image
        QString avatarPath = avatar.getFullPath();

        if (QFile::exists(avatarPath)) {
            QPixmap pixmap(avatarPath);
            if (!pixmap.isNull()) {
                // Scale to fit the circular area
                QPixmap scaled = pixmap.scaled(120, 120, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

                // Create circular pixmap
                QPixmap circular(120, 120);
                circular.fill(Qt::transparent);
                QPainter painter(&circular);
                painter.setRenderHint(QPainter::Antialiasing);

                QPainterPath path;
                path.addEllipse(0, 0, 120, 120);
                painter.setClipPath(path);

                // Draw scaled pixmap centered
                int x = (scaled.width() - 120) / 2;
                int y = (scaled.height() - 120) / 2;
                painter.drawPixmap(-x, -y, scaled);
                painter.end();

                avatarLabel->setPixmap(circular);
                return;
            }
        }
    }

    // Default avatar: show first letter of username
    QPixmap defaultAvatar(120, 120);
    defaultAvatar.fill(Qt::lightGray);
    QPainter painter(&defaultAvatar);
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(48);
    font.setBold(true);
    painter.setFont(font);

    // Get first letter of username or default "?"
    QString initial = m_currentUser.getUsername().isEmpty() ? "?" : m_currentUser.getUsername().left(1).toUpper();
    painter.drawText(defaultAvatar.rect(), Qt::AlignCenter, initial);
    painter.end();

    // Make it circular
    QPixmap circular(120, 120);
    circular.fill(Qt::transparent);
    QPainter circlePainter(&circular);
    circlePainter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addEllipse(0, 0, 120, 120);
    circlePainter.setClipPath(path);
    circlePainter.drawPixmap(0, 0, defaultAvatar);
    circlePainter.end();

    avatarLabel->setPixmap(circular);
}

void ProfileScreen::updateDisplay() {
    auto& session = Core::Entities::Session::instance();

    welcomeLabel->setText("Welcome, " + m_currentUser.getUsername() + "!");
    userIdLabel->setText("User ID: " + QString::number(m_currentUser.getId()));
    sessionIdLabel->setText("Session ID: " + session.getSessionId().left(8) + "...");

    qint64 duration = session.getSessionDuration();
    sessionDurationLabel->setText("Session Duration: " + QString::number(duration / 60) + " minutes, " +
                                   QString::number(duration % 60) + " seconds");
}

void ProfileScreen::onRefreshClicked() {
    // Refresh session
    Core::Entities::Session::instance().refreshSession();

    // Refresh user data from database
    UserDao userDao;
    auto userFromDb = userDao.findUserById(m_currentUser.getId());

    if (userFromDb.has_value()) {
        m_currentUser = userFromDb.value();
        updateDisplay();           // Refresh username, email, etc.
        updateAvatarDisplay();     // Refresh avatar
    }

    LOG_INFO("Session and user data refreshed");
    QMessageBox::information(this, "Refresh", "Session and profile have been refreshed!");
}



void ProfileScreen::onLogoutClicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Logout",
        "Are you sure you want to logout?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        emit logout();
        this->close();
    }
}