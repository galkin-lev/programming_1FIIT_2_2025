//
// Created by lv on 2/3/26.
//

#include "loginScreen.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QFont>
#include "../../../logger/logger.h"
#include "../../../authorization/data/local_db/db.h"
#include "../../../authorization/data/local_db/UserDao.h"
#include "loginScreen.h"



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




    if (!initDatabase()) {
        std::cerr << "Failed to initialize database" << std::endl;
        LOG_FATAL("failed to initialize database");
    }

    UserDao userDao;


    /////////////////////////////////////////////////////// test
    // // Save some users
    // User user1;
    // user1.username = "alice";
    // user1.password = "pass123";
    // user1.id = -1;  // Will auto-generate ID
    //
    // User user2;
    // user2.username = "bob";
    // user2.password = "secret456";
    // user2.id = -1;

    // LOG_INFO("ready to save:" + user2.username + " " + user2.password);
    //
    // if (userDao.saveUser(user1)) {
    //     LOG_TRACE("User 1 added to the db");
    // }
    //
    // if (userDao.saveUser(user2)) {
    //     LOG_TRACE("User 2 added to the db");
    // }

    // Load all users
    std::vector<User> allUsers = userDao.loadAllUsers();

    std::cout << "\nAll users in database:" << std::endl;
    for (const auto& user : allUsers) {
        std::cout << "ID: " << user.id
                  << ", Username: " << user.username
                  << ", Password: " << user.password << std::endl;
    }
///////////////////////////////////////


    // if (userDao.findUserByUsername(usernameTextField->text().toStdString())) {
    //
    // }

    if (userDao.validateUserCredentials(usernameTextField->text().toStdString(), passwordTextField->text().toStdString())) {
        LOG_INFO("Login successful for: " + usernameTextField->text().toStdString());
        QMessageBox::information(this, "Success", "Login successful!");

        // Open main application window here
        // mainWindow->show();
        // this->hide();
    } else {
        LOG_WARN("Login failed for: " + usernameTextField->text().toStdString());
        QMessageBox::warning(this, "Error", "Invalid username or password!");
        passwordTextField->clear();
        passwordTextField->setFocus();
    }





    QString text = usernameTextField->text().isEmpty() ?
                  "No text entered" : usernameTextField->text();

    windowCountLabel->setText("You entered: " + text);

    LOG_INFO(" (loginBtn)  | text enetered: " + text.toStdString() +
           "\nTotal windows opened: " + QString::number(windowCounter).toStdString() );

    QMessageBox::information(this, "Message",
        "You entered: " + text + "\n\n" +
        "Total windows opened: " + QString::number(windowCounter));






}

void loginScreen::clearText() {
    // Реализация в connectSignals через lambda
}