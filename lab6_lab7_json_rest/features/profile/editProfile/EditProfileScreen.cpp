//
// Created by lv on 5/10/26.
//

//
// Created by lv on 5/10/26.
//

#include "EditProfileScreen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include "core/data/local_db/dao/UserDao.h"
#include "../../../logger/logger.h"
#include "features/profile/photoPicker/PhotoPickerScreen.h"

EditProfileScreen::EditProfileScreen(QWidget* parent)
    : QMainWindow(parent) {
    setupUI();
}

void EditProfileScreen::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Title
    QLabel* titleLabel = new QLabel("Edit Profile", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(20);

    // Form layout
    QFormLayout* formLayout = new QFormLayout();
    formLayout->setSpacing(15);
    formLayout->setLabelAlignment(Qt::AlignRight);

    // Username
    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("Enter username");
    m_usernameEdit->setMinimumWidth(250);
    formLayout->addRow("Username:", m_usernameEdit);


    // Separator
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    formLayout->addRow(line);

    // Password fields
    m_currentPasswordEdit = new QLineEdit(this);
    m_currentPasswordEdit->setEchoMode(QLineEdit::Password);
    m_currentPasswordEdit->setPlaceholderText("Required to save changes");
    formLayout->addRow("Current Password:", m_currentPasswordEdit);

    m_newPasswordEdit = new QLineEdit(this);
    m_newPasswordEdit->setEchoMode(QLineEdit::Password);
    m_newPasswordEdit->setPlaceholderText("Leave empty to keep current");
    formLayout->addRow("New Password:", m_newPasswordEdit);

    m_confirmPasswordEdit = new QLineEdit(this);
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    m_confirmPasswordEdit->setPlaceholderText("Confirm new password");
    formLayout->addRow("Confirm Password:", m_confirmPasswordEdit);

    mainLayout->addLayout(formLayout);

    // Error label
    m_errorLabel = new QLabel(this);
    m_errorLabel->setStyleSheet("color: red;");
    m_errorLabel->setWordWrap(true);
    m_errorLabel->hide();
    mainLayout->addWidget(m_errorLabel);

    mainLayout->addSpacing(20);

    // Buttons
    m_managePhotosBtn = new QPushButton("📸 Manage Photos", this);
    m_managePhotosBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #FF9800;"
        "  color: white;"
        "  padding: 10px;"
        "  font-size: 14px;"
        "  border-radius: 5px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #F57C00;"
        "}"
    );

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    m_saveBtn = new QPushButton("Save Changes", this);
    m_saveBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #4CAF50;"
        "  color: white;"
        "  padding: 10px 20px;"
        "  border-radius: 5px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #45a049;"
        "}"
    );

    m_cancelBtn = new QPushButton("Cancel", this);
    m_cancelBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #9E9E9E;"
        "  color: white;"
        "  padding: 10px 20px;"
        "  border-radius: 5px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #757575;"
        "}"
    );

    buttonLayout->addWidget(m_saveBtn);
    buttonLayout->addWidget(m_cancelBtn);

    mainLayout->addWidget(m_managePhotosBtn);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    setCentralWidget(centralWidget);
    setWindowTitle("Edit Profile");
    resize(450, 550);

    // Connect signals
    connect(m_saveBtn, &QPushButton::clicked, this, &EditProfileScreen::onSaveClicked);
    connect(m_cancelBtn, &QPushButton::clicked, this, &EditProfileScreen::onCancelClicked);
    connect(m_managePhotosBtn, &QPushButton::clicked, this, &EditProfileScreen::onManagePhotosClicked);
}

void EditProfileScreen::onManagePhotosClicked() {
    // Create and show photo picker screen directly
    PhotoPickerScreen* photoPicker = new PhotoPickerScreen(this);
    photoPicker->setUser(m_currentUser);
    photoPicker->show();

    // this->close();
}

void EditProfileScreen::setUser(const Core::Entities::User& user) {
    m_currentUser = user;
    loadUserData();
}

void EditProfileScreen::loadUserData() {
    m_usernameEdit->setText(m_currentUser.getUsername());
    // m_emailEdit->setText(m_currentUser);
}

bool EditProfileScreen::validateInputs() {
    // Clear error
    m_errorLabel->hide();

    // Validate username
    if (m_usernameEdit->text().trimmed().isEmpty()) {
        m_errorLabel->setText("Username cannot be empty");
        m_errorLabel->show();
        return false;
    }

    if (m_usernameEdit->text().length() < 3) {
        m_errorLabel->setText("Username must be at least 3 characters");
        m_errorLabel->show();
        return false;
    }



    // Validate current password
    if (m_currentPasswordEdit->text().isEmpty()) {
        m_errorLabel->setText("Current password is required to save changes");
        m_errorLabel->show();
        return false;
    }

    // Validate new password if provided
    QString newPassword = m_newPasswordEdit->text();
    if (!newPassword.isEmpty()) {
        if (newPassword.length() < 6) {
            m_errorLabel->setText("New password must be at least 6 characters");
            m_errorLabel->show();
            return false;
        }

        if (newPassword != m_confirmPasswordEdit->text()) {
            m_errorLabel->setText("New passwords do not match");
            m_errorLabel->show();
            return false;
        }
    }

    return true;
}

void EditProfileScreen::onSaveClicked() {
    if (!validateInputs()) {
        return;
    }

    UserDao userDao;

    // Verify current password
    if (!userDao.validateUserCredentials(m_currentUser.getUsername(), m_currentPasswordEdit->text())) {
        QMessageBox::warning(this, "Error", "Current password is incorrect");
        return;
    }

    LOG_INFO("Current password verified");

    // Update user data
    Core::Entities::User updatedUser = m_currentUser;
    updatedUser.setUsername(m_usernameEdit->text().trimmed());

    LOG_INFO("Username updated, setting new password...");

    // Update password if provided
    QString newPassword = m_newPasswordEdit->text();
    if (!newPassword.isEmpty()) {
        try {
            LOG_INFO("Attempting to set new password (length: " + std::to_string(newPassword.length()) + ")");
            updatedUser.updatePassword(newPassword);
            LOG_INFO("Password updated successfully in user object");
        } catch (const std::exception& e) {
            LOG_ERROR("Exception setting password: " + std::string(e.what()));
            QMessageBox::warning(this, "Error", "Failed to set password: " + QString(e.what()));
            return;
        }
    }

    // Save to database
    LOG_INFO("Saving to database...");
    if (userDao.updateUser(updatedUser)) {
        QMessageBox::information(this, "Success", "Profile updated successfully!");
        emit profileUpdated();
        this->close();
    } else {
        QMessageBox::warning(this, "Error", "Failed to update profile");
    }
}

void EditProfileScreen::onCancelClicked() {
    this->close();
}

