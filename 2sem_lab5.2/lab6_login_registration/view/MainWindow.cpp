//
// Created by lv on 2/3/26.
//

#include "MainWindow.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QFont>

#include "../logger/logger.h"
#include "../core/data/local_db/db.h"
#include "../core/data/local_db/dao/UserDao.h"
#include "features/profile/ProfileScreen.h"
#include "features/profile/editProfile/EditProfileScreen.h"
#include "features/profile/photoPicker/PhotoPickerScreen.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    connectSignals();

    LOG_INFO("main window was created");
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Заголовок
    QLabel* titleLabel = new QLabel("Simple Window Manager", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // Информационная метка
    windowCountLabel = new QLabel("No extra windows open", this);
    windowCountLabel->setAlignment(Qt::AlignCenter);

    // Текстовое поле
    textField = new QLineEdit(this);
    textField->setPlaceholderText("Enter some text here...");
    textField->setMaximumWidth(300);

    // Кнопки
    openWindowBtn = new QPushButton("Open New Window", this);
    closeWindowBtn = new QPushButton("Close Extra Window", this);
    showMessageBtn = new QPushButton("Show Message", this);
    clearTextBtn = new QPushButton("Clear Text", this);

    // Стилизация кнопок
    openWindowBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   padding: 8px;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
    );

    closeWindowBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #f44336;"
        "   color: white;"
        "   padding: 8px;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #d32f2f;"
        "}"
    );

    showMessageBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #2196F3;"
        "   color: white;"
        "   padding: 8px;"
        "   border-radius: 4px;"
        "}"
    );

    clearTextBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #FF9800;"
        "   color: white;"
        "   padding: 8px;"
        "   border-radius: 4px;"
        "}"
    );

    // Добавляем виджеты в layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(windowCountLabel);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(textField, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(openWindowBtn);
    mainLayout->addWidget(closeWindowBtn);
    mainLayout->addWidget(showMessageBtn);
    mainLayout->addWidget(clearTextBtn);
    mainLayout->addStretch();

    // Начальное состояние кнопки закрытия
    closeWindowBtn->setEnabled(false);

    setCentralWidget(centralWidget);
    setWindowTitle("Simple GUI Application");
    resize(400, 400);
}

void MainWindow::connectSignals() {
    // Открытие нового окна
    connect(openWindowBtn, &QPushButton::clicked, this, &MainWindow::openNewWindow);

    // Закрытие окна
    connect(closeWindowBtn, &QPushButton::clicked, this, &MainWindow::closeExtraWindow);

    // Show message button
    connect(showMessageBtn, &QPushButton::clicked, this, &MainWindow::showMessage);

    // Clear Text button
    connect(clearTextBtn, &QPushButton::clicked, this, [this]() {
        textField->clear();
        textField->setFocus();
        LOG_INFO("Text field cleared");
    });
}

void MainWindow::openNewWindow() {
    windowCounter++;

    // Закрываем предыдущее окно, если оно есть
    if (currentExtraWindow) {
        currentExtraWindow->close();
        currentExtraWindow->deleteLater();
    }

    // Создаем новое окно
    currentExtraWindow = new QWidget(this, Qt::Window);
    currentExtraWindow->setWindowTitle("Extra Window #" + QString::number(windowCounter));
    currentExtraWindow->resize(300, 200);

    QVBoxLayout* layout = new QVBoxLayout(currentExtraWindow);

    QLabel* label = new QLabel(
        "This is extra window #" + QString::number(windowCounter) +
        "\n\nIt will close when you:\n"
        "1. Click 'Close Extra Window'\n"
        "2. Or close this window manually",
        currentExtraWindow
    );
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);

    QPushButton* closeBtn = new QPushButton("Close Me", currentExtraWindow);
    QPushButton* errorBtn = new QPushButton("Simulate Error", currentExtraWindow);

    layout->addWidget(label);
    layout->addWidget(closeBtn);
    layout->addWidget(errorBtn);

    currentExtraWindow->setLayout(layout);
    currentExtraWindow->show();

    windowCountLabel->setText("Extra window #" + QString::number(windowCounter) + " is open");
    closeWindowBtn->setEnabled(true);

    connect(closeBtn, &QPushButton::clicked, [this]() {
        closeExtraWindow();
    });

    connect(errorBtn, &QPushButton::clicked, [this]() {
        try {
            throw std::runtime_error("Try/catch simulated error");
        } catch(const std::exception& e) {
            LOG_ERROR("ERROR: " + string(e.what()));
            QMessageBox::critical(currentExtraWindow, "Error",
                "Simulated error!\n\n"
                "This is where you would add error logging.\n"
                "Check the console for error messages.");
        }
    });

    connect(currentExtraWindow, &QWidget::destroyed, this, [this]() {
        windowCountLabel->setText("Window was closed manually");
        closeWindowBtn->setEnabled(false);
        currentExtraWindow = nullptr;
        LOG_WARN("Extra window closed manually");
    });

    LOG_INFO("New window opened: #" + std::to_string(windowCounter));
}



void MainWindow::closeExtraWindow() {
    LOG_TRACE("Trace log example");

    if (currentExtraWindow) {
        LOG_INFO("Closing extra window");
        currentExtraWindow->close();
        currentExtraWindow->deleteLater();
        currentExtraWindow = nullptr;
        windowCountLabel->setText("No extra windows open");
        closeWindowBtn->setEnabled(false);
    } else {
        LOG_WARN("No extra windows opened");
    }
}

void MainWindow::showMessage() {
    UserDao userDao;

    // Load all users
    std::vector<Core::Entities::User> allUsers = userDao.loadAllUsers();

    std::cout << "\nAll users in database:" << std::endl;
    for (const auto& user : allUsers) {
        std::cout << "ID: " << user.getId()
                  << ", Username: " << user.getUsername().toStdString()
                  << ", Password: " << user.getPasswordHash().toStdString() << std::endl;
    }

    QString text = textField->text().isEmpty() ?
                  "No text entered" : textField->text();

    windowCountLabel->setText("You entered: " + text);

    LOG_INFO("(showMessageBtn) | text entered: " + text.toStdString() +
             "\nTotal windows opened: " + QString::number(windowCounter).toStdString());

    QMessageBox::information(this, "Message",
        "You entered: " + text + "\n\n" +
        "Total windows opened: " + QString::number(windowCounter));
}

void MainWindow::onEditProfile() {
    if (!editProfileScreen) {
        editProfileScreen = new EditProfileScreen(this);
        connect(editProfileScreen, &EditProfileScreen::profileUpdated, this, &MainWindow::onAvatarUpdated);
        connect(editProfileScreen, &EditProfileScreen::openPhotoPicker, this, &MainWindow::onOpenPhotoPicker);
    }

    // Set current user in edit profile screen
    editProfileScreen->setUser(m_currentUser);
    editProfileScreen->show();
}

void MainWindow::onOpenPhotoPicker() {
    if (!photoPickerScreen) {
        photoPickerScreen = new PhotoPickerScreen(this);
        connect(photoPickerScreen, &PhotoPickerScreen::avatarUpdated, this, &MainWindow::onAvatarUpdated);
        connect(photoPickerScreen, &PhotoPickerScreen::backToProfile, [this]() {
            if (photoPickerScreen) {
                photoPickerScreen->hide();
            }
        });
    }

    // Set current user in photo picker screen
    photoPickerScreen->setUser(m_currentUser);
    photoPickerScreen->show();
}

void MainWindow::onAvatarUpdated() {
    // Refresh avatar in profile screen
    if (profileScreen) {
        profileScreen->refreshAvatar();
    }

    // Also refresh current user data
    UserDao userDao;
    auto userFromDb = userDao.findUserById(m_currentUser.getId());
    if (userFromDb.has_value()) {
        m_currentUser = userFromDb.value();
    }
}