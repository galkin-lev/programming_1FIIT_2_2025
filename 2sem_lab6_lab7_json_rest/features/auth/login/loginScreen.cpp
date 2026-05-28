//
// Created by lv on 2/3/26.
//

#include "loginScreen.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QFont>
#include "../../../logger/logger.h"
#include "core/data/local_db/db.h"
#include "core/data/local_db/dao/UserDao.h"
#include "loginScreen.h"
#include "core/model/User.h"
#include "core/model/Session.h"
#include "core/model/Photo.h"

#include "core/model/Session.h"


namespace Core::Entities {
    class User;
}

loginScreen::loginScreen(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    connectSignals();

    LOG_INFO("main window was created");
}

void loginScreen::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Заголовок
    QLabel* titleLabel = new QLabel("login", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // Информационная метка
    windowCountLabel = new QLabel("No extra windows open", this);
    windowCountLabel->setAlignment(Qt::AlignCenter);

    // Текстовое поле
    usernameTextField = new QLineEdit(this);
    usernameTextField->setPlaceholderText("Enter username");
    usernameTextField->setMaximumWidth(300);

    passwordTextField = new QLineEdit(this);
    passwordTextField->setPlaceholderText("Enter password");
    passwordTextField->setEchoMode(QLineEdit::Password);
    passwordTextField->setMaximumWidth(300);

    // Кнопки
    registerBtn = new QPushButton("Register", this);
    // closeWindowBtn = new QPushButton("Close Extra Window", this);
    loginBtn = new QPushButton("Login", this);
    clearTextBtn = new QPushButton("Clear Text", this);

    loginBtn->setStyleSheet(
           "QPushButton {"
           "   background-color: #2196F3;"
           "   color: white;"
           "   padding: 8px;"
           "   border-radius: 4px;"
           "}"
       );

    // Стилизация кнопок
    registerBtn->setStyleSheet(
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

    // closeWindowBtn->setStyleSheet(
    //     "QPushButton {"
    //     "   background-color: #f44336;"
    //     "   color: white;"
    //     "   padding: 8px;"
    //     "   border-radius: 4px;"
    //     "}"
    //     "QPushButton:hover {"
    //     "   background-color: #d32f2f;"
    //     "}"
    // );



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

    mainLayout->addSpacing(20);
    mainLayout->addWidget(usernameTextField, 0, Qt::AlignCenter);
    mainLayout->addWidget(passwordTextField, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(loginBtn);
    mainLayout->addWidget(registerBtn);
    // mainLayout->addWidget(closeWindowBtn);
    mainLayout->addWidget(clearTextBtn);
    mainLayout->addStretch();
    mainLayout->addWidget(windowCountLabel);

    // Начальное состояние кнопки закрытия
    // closeWindowBtn->setEnabled(false);

    setCentralWidget(centralWidget);
    setWindowTitle("Simple GUI Application");
    resize(400, 400);
}

void loginScreen::connectSignals() {
    // Открытие нового окна
    connect(registerBtn, &QPushButton::clicked, this, &loginScreen::openRegistration);

    // Закрытие окна
    // connect(closeWindowBtn, &QPushButton::clicked, this, &loginScreen::closeExtraWindow);

    // Кнопка Show Message    // Кнопка Show Message

    connect(loginBtn, &QPushButton::clicked, this, &loginScreen::login);



    // Кнопка Clear Text
    connect(clearTextBtn, &QPushButton::clicked, this, [this]() {
        usernameTextField->clear();
        usernameTextField->setFocus();

        // logging::trace("Text field cleared"); // Будет добавлено позже
    });
}

void loginScreen::openRegistration() {
    if (!registrationWindow) {
        registrationWindow = new registrationScreen(this);

        // Clean up when closed
        connect(registrationWindow, &registrationScreen::destroyed, this, [this]() {
            registrationWindow = nullptr;
            LOG_INFO("Registration window destroyed");
        });

        // Optional: center relative to parent
        registrationWindow->move(this->x() + 50, this->y() + 50);
    }

    registrationWindow->show();
    registrationWindow->raise();
    registrationWindow->activateWindow();

    LOG_INFO("Registration window opened");
}

void loginScreen::openNewWindow() {
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

    // Текстовое поле
    usernameTextField = new QLineEdit(this);
    usernameTextField->setPlaceholderText("Enter username");
    usernameTextField->setMaximumWidth(300);

    passwordTextField = new QLineEdit(this);
    passwordTextField->setPlaceholderText("Enter password");
    passwordTextField->setMaximumWidth(300);

    // Заголовок
    QLabel* titleLabel = new QLabel("registration", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

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
    layout->addWidget(titleLabel);
    layout->addSpacing(20);

    layout->addSpacing(20);
    layout->addWidget(usernameTextField, 0, Qt::AlignCenter);
    layout->addWidget(passwordTextField, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(loginBtn);
    layout->addWidget(registerBtn);
    layout->addWidget(closeBtn);
    layout->addWidget(errorBtn);

    currentExtraWindow->setLayout(layout);
    currentExtraWindow->show();

    windowCountLabel->setText("Extra window #" + QString::number(windowCounter) + " is open");
    // closeWindowBtn->setEnabled(true);

    connect(closeBtn, &QPushButton::clicked, [this]() {
        closeExtraWindow();
    });

    connect(errorBtn, &QPushButton::clicked, [this]() {

            try {
                throw std::runtime_error(" Try/catch simulated error ");

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
        // closeWindowBtn->setEnabled(false);
        currentExtraWindow = nullptr;

        LOG_WARN("Extra window closed manually");

    });

    LOG_INFO("New window opened: #" + std::to_string(windowCounter));
}

void loginScreen::closeExtraWindow() {
    LOG_TRACE("Trace log example djfksfdkskf");
    LOG_WARN("No extra windows opened");


    if (currentExtraWindow) {

        logging::debug("Closing extra window");

        currentExtraWindow->close();
        currentExtraWindow->deleteLater();        currentExtraWindow = nullptr;

        windowCountLabel->setText("No extra windows open");
        // closeWindowBtn->setEnabled(false);

    } else {


    }
}

void loginScreen::login() {


    UserDao userDao;

////////////////// show all users in the databse /////////////////////////
    std::vector<Core::Entities::User> allUsers = userDao.loadAllUsers();

    std::cout << "\nAll users in database:" << std::endl;
    for (const auto& user : allUsers) {
        std::cout << "ID: " << user.getId()
                  << ", Username: " << user.getUsername().toStdString()
                  << ", Password: " << user.getPasswordHash().toStdString() << std::endl;
    }
////////////////////////////////////////////////////////////////////////////



    if (userDao.validateUserCredentials(usernameTextField->text(), passwordTextField->text())) {
        LOG_INFO("Login successful for: " + usernameTextField->text().toStdString());

        // Get the full user object
        auto userOpt = userDao.findUserByUsername(usernameTextField->text());
        if (userOpt.has_value()) {
            // Start the session
            Core::Entities::Session::instance().startSession(userOpt.value());
            LOG_INFO("Session started for user: " + usernameTextField->text().toStdString());

            // Open profile screen instead of showing message box
            openProfileScreen();
        } else {
            QMessageBox::warning(this, "Error", "Could not load user data!");
        }
    } else {
        LOG_WARN("Login failed for: " + usernameTextField->text().toStdString());
        QMessageBox::warning(this, "Error", "Invalid username or password!");
        passwordTextField->clear();
        passwordTextField->setFocus();
    }
}

void loginScreen::openProfileScreen() {
    // Get the current user from session
    Core::Entities::User currentUser = Core::Entities::Session::instance().getCurrentUser();

    // Create and show profile screen
    if (!profileScreen) {
        profileScreen = new ProfileScreen(this);
        connect(profileScreen, &ProfileScreen::logout, this, &loginScreen::onLogout);
        connect(profileScreen, &ProfileScreen::destroyed, this, [this]() {
            profileScreen = nullptr;
        });
    }

    profileScreen->setUser(currentUser);
    profileScreen->show();
    this->hide();  // Hide login window
}

void loginScreen::onLogout() {
    Core::Entities::Session::instance().endSession();
    LOG_INFO("User logged out, session ended");
    this->show();  // Show login window again
    usernameTextField->clear();
    passwordTextField->clear();
    usernameTextField->setFocus();
}

void loginScreen::clearText() {
    // Реализация в connectSignals через lambda
}