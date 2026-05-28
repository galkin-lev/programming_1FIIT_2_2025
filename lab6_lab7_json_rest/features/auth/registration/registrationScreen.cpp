//
// Created by lv on 2/3/26.
//

#include "registrationScreen.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QFont>
#include "logger/logger.h"
#include <QTimer>



#include "registrationScreen.h"
#include "core/data/local_db/dao/UserDao.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFont>
#include <QRegularExpressionValidator>

#include <QVariant>
#include <QSqlError>


#include <QRegularExpressionValidator>
#include <QVariant>
#include <QSqlError>




registrationScreen::registrationScreen(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    connectSignals();

    LOG_INFO("Registration screen created");
}

registrationScreen::~registrationScreen() {
    LOG_INFO("Registration screen destroyed");
}

void registrationScreen::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Title
    QLabel* titleLabel = new QLabel("Create New Account", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // Username field
    usernameTextField = new QLineEdit(this);
    usernameTextField->setPlaceholderText("Choose username");
    usernameTextField->setMaximumWidth(300);

    // Password field
    passwordTextField = new QLineEdit(this);
    passwordTextField->setPlaceholderText("Choose password");
    passwordTextField->setEchoMode(QLineEdit::Password);
    passwordTextField->setMaximumWidth(300);

    // Confirm password field
    confirmPasswordTextField = new QLineEdit(this);
    confirmPasswordTextField->setPlaceholderText("Confirm password");
    confirmPasswordTextField->setEchoMode(QLineEdit::Password);
    confirmPasswordTextField->setMaximumWidth(300);

    // Password strength indicator
    QLabel* passwordHint = new QLabel("Password must be at least 6 characters", this);
    passwordHint->setStyleSheet("color: gray; font-size: 10px;");
    passwordHint->setAlignment(Qt::AlignCenter);

    // Status label for feedback
    statusLabel = new QLabel(this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setWordWrap(true);

    // Buttons
    registerBtn = new QPushButton("Register", this);
    registerBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   padding: 8px;"
        "   border-radius: 4px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
    );

    cancelBtn = new QPushButton("Cancel", this);
    cancelBtn->setStyleSheet(
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

    // Button layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(registerBtn);
    buttonLayout->addWidget(cancelBtn);

    // Add all widgets to main layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(usernameTextField, 0, Qt::AlignCenter);
    mainLayout->addWidget(passwordTextField, 0, Qt::AlignCenter);
    mainLayout->addWidget(confirmPasswordTextField, 0, Qt::AlignCenter);
    mainLayout->addWidget(passwordHint, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(statusLabel, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    setCentralWidget(centralWidget);
    setWindowTitle("Registration - Create New Account");
    resize(400, 450);

    // Set window flags to make it a separate window
    setWindowFlags(Qt::Window);
}

void registrationScreen::connectSignals() {
    connect(registerBtn, &QPushButton::clicked, this, &registrationScreen::onRegisterClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &registrationScreen::onCancelClicked);

    // Enable/disable register button based on input
    auto updateButtonState = [this]() {
        registerBtn->setEnabled(!usernameTextField->text().isEmpty() &&
                               !passwordTextField->text().isEmpty() &&
                               !confirmPasswordTextField->text().isEmpty());
    };

    connect(usernameTextField, &QLineEdit::textChanged, this, updateButtonState);
    connect(passwordTextField, &QLineEdit::textChanged, this, updateButtonState);
    connect(confirmPasswordTextField, &QLineEdit::textChanged, this, updateButtonState);

    // Initially disable register button
    registerBtn->setEnabled(false);
}

void registrationScreen::onRegisterClicked() {
    QString username = usernameTextField->text().trimmed();
    QString password = passwordTextField->text();
    QString confirmPassword = confirmPasswordTextField->text();

    // Validation
    if (username.isEmpty()) {
        statusLabel->setText("❌ Username cannot be empty");
        statusLabel->setStyleSheet("color: red;");
        LOG_WARN("Registration: Empty username");
        return;
    }

    if (password.length() < 6) {
        statusLabel->setText("❌ Password must be at least 6 characters");
        statusLabel->setStyleSheet("color: red;");
        LOG_WARN("Registration: Password too short");
        return;
    }

    if (password != confirmPassword) {
        statusLabel->setText("❌ Passwords do not match");
        statusLabel->setStyleSheet("color: red;");
        LOG_WARN("Registration: Passwords don't match");
        return;
    }

    LOG_INFO("Attempting to register user: " + username.toStdString());



    UserDao userDao;

    // Check if username already exists
    if (userDao.userExists(username)) {
        statusLabel->setText("❌ Username already exists!");
        statusLabel->setStyleSheet("color: red;");
        LOG_WARN("Registration failed: Username already exists - " + username.toStdString());
        usernameTextField->clear();
        usernameTextField->setFocus();
        return;
    }

    // ✅ FIX: Use static factory method correctly
    // createNewUser is a STATIC method that returns a User object
    Core::Entities::User newUser = Core::Entities::User::createNewUser(username, password);

    LOG_DEBUG("Created user object - Username: " + newUser.getUsername().toStdString());
    LOG_DEBUG("Password hash length: " + std::to_string(newUser.getPasswordHash().length()));

    // Save to database
    if (userDao.saveUser(newUser)) {
        statusLabel->setText("✅ Registration successful! You can now login.");
        statusLabel->setStyleSheet("color: green;");
        LOG_INFO("User registered successfully: " + username.toStdString());

        // Clear fields
        usernameTextField->clear();
        passwordTextField->clear();
        confirmPasswordTextField->clear();

        // Show success message
        QMessageBox::information(this, "Success",
            "Registration successful!\n\nYou can now login with your credentials.");

        // Close registration window after short delay
        QTimer::singleShot(1000, this, &registrationScreen::close);
    } else {
        statusLabel->setText("❌ Registration failed! Database error.");
        statusLabel->setStyleSheet("color: red;");
        LOG_ERROR("Registration failed for user: " + username.toStdString());

        QMessageBox::warning(this, "Error",
            "Failed to register user.\nPlease check logs for details.");
    }
}

void registrationScreen::onCancelClicked() {
    LOG_INFO("Registration cancelled by user");
    close();
}

void registrationScreen::onCloseWindow() {
    LOG_INFO("Registration closed");
}






//
// registrationScreen::registrationScreen(QWidget* parent) : QMainWindow(parent) {
//     setupUI();
//     connectSignals();
//
//     LOG_INFO("Registration screen created");
// }
//
// registrationScreen::~registrationScreen() {
//     LOG_INFO("Registration screen destroyed");
// }
//
// void registrationScreen::setupUI() {
//     QWidget* centralWidget = new QWidget(this);
//     QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
//
//     // Title
//     QLabel* titleLabel = new QLabel("Create New Account", this);
//     titleLabel->setAlignment(Qt::AlignCenter);
//     QFont titleFont = titleLabel->font();
//     titleFont.setPointSize(18);
//     titleFont.setBold(true);
//     titleLabel->setFont(titleFont);
//
//     // Username field
//     usernameTextField = new QLineEdit(this);
//     usernameTextField->setPlaceholderText("Choose username");
//     usernameTextField->setMaximumWidth(300);
//
//     // Password field
//     passwordTextField = new QLineEdit(this);
//     passwordTextField->setPlaceholderText("Choose password");
//     passwordTextField->setEchoMode(QLineEdit::Password);
//     passwordTextField->setMaximumWidth(300);
//
//     // Confirm password field
//     confirmPasswordTextField = new QLineEdit(this);
//     confirmPasswordTextField->setPlaceholderText("Confirm password");
//     confirmPasswordTextField->setEchoMode(QLineEdit::Password);
//     confirmPasswordTextField->setMaximumWidth(300);
//
//     // Password strength indicator (optional)
//     QLabel* passwordHint = new QLabel("Password must be at least 6 characters", this);
//     passwordHint->setStyleSheet("color: gray; font-size: 10px;");
//     passwordHint->setAlignment(Qt::AlignCenter);
//
//     // Status label for feedback
//     statusLabel = new QLabel(this);
//     statusLabel->setAlignment(Qt::AlignCenter);
//     statusLabel->setWordWrap(true);
//
//     // Buttons
//     registerBtn = new QPushButton("Register", this);
//     registerBtn->setStyleSheet(
//         "QPushButton {"
//         "   background-color: #4CAF50;"
//         "   color: white;"
//         "   padding: 8px;"
//         "   border-radius: 4px;"
//         "   font-weight: bold;"
//         "}"
//         "QPushButton:hover {"
//         "   background-color: #45a049;"
//         "}"
//     );
//
//     cancelBtn = new QPushButton("Cancel", this);
//     cancelBtn->setStyleSheet(
//         "QPushButton {"
//         "   background-color: #f44336;"
//         "   color: white;"
//         "   padding: 8px;"
//         "   border-radius: 4px;"
//         "}"
//         "QPushButton:hover {"
//         "   background-color: #d32f2f;"
//         "}"
//     );
//
//     // Button layout
//     QHBoxLayout* buttonLayout = new QHBoxLayout();
//     buttonLayout->addWidget(registerBtn);
//     buttonLayout->addWidget(cancelBtn);
//
//     // Add all widgets to main layout
//     mainLayout->addWidget(titleLabel);
//     mainLayout->addSpacing(20);
//     mainLayout->addWidget(usernameTextField, 0, Qt::AlignCenter);
//     mainLayout->addWidget(passwordTextField, 0, Qt::AlignCenter);
//     mainLayout->addWidget(confirmPasswordTextField, 0, Qt::AlignCenter);
//     mainLayout->addWidget(passwordHint, 0, Qt::AlignCenter);
//     mainLayout->addSpacing(10);
//     mainLayout->addLayout(buttonLayout);
//     mainLayout->addWidget(statusLabel, 0, Qt::AlignCenter);
//     mainLayout->addStretch();
//
//     setCentralWidget(centralWidget);
//     setWindowTitle("Registration - Create New Account");
//     resize(400, 450);
//
//     // Set window flags to make it a separate window
//     setWindowFlags(Qt::Window);
// }
//
// void registrationScreen::connectSignals() {
//     connect(registerBtn, &QPushButton::clicked, this, &registrationScreen::onRegisterClicked);
//     connect(cancelBtn, &QPushButton::clicked, this, &registrationScreen::onCancelClicked);
//
//     // Optional: Enable/disable register button based on input
//     connect(usernameTextField, &QLineEdit::textChanged, this, [this]() {
//         registerBtn->setEnabled(!usernameTextField->text().isEmpty() &&
//                                !passwordTextField->text().isEmpty() &&
//                                !confirmPasswordTextField->text().isEmpty());
//     });
//     connect(passwordTextField, &QLineEdit::textChanged, this, [this]() {
//         registerBtn->setEnabled(!usernameTextField->text().isEmpty() &&
//                                !passwordTextField->text().isEmpty() &&
//                                !confirmPasswordTextField->text().isEmpty());
//     });
//     connect(confirmPasswordTextField, &QLineEdit::textChanged, this, [this]() {
//         registerBtn->setEnabled(!usernameTextField->text().isEmpty() &&
//                                !passwordTextField->text().isEmpty() &&
//                                !confirmPasswordTextField->text().isEmpty());
//     });
//
//     // Initially disable register button
//     registerBtn->setEnabled(false);
// }
//
// void registrationScreen::onRegisterClicked() {
//     QString username = usernameTextField->text().trimmed();
//     QString password = passwordTextField->text();
//     QString confirmPassword = confirmPasswordTextField->text();
//
//     // Validation
//     if (username.isEmpty()) {
//         statusLabel->setText("❌ Username cannot be empty");
//         statusLabel->setStyleSheet("color: red;");
//         return;
//     }
//
//     if (password.length() < 6) {
//         statusLabel->setText("❌ Password must be at least 6 characters");
//         statusLabel->setStyleSheet("color: red;");
//         return;
//     }
//
//     if (password != confirmPassword) {
//         statusLabel->setText("❌ Passwords do not match");
//         statusLabel->setStyleSheet("color: red;");
//         return;
//     }
//
//     LOG_INFO("Attempting to register user: " + username.toStdString());
//

//
//     UserDao userDao;
//
//     // Check if username already exists
//     auto existingUser = userDao.findUserByUsername(username);
//     if (existingUser.has_value()) {
//         statusLabel->setText("❌ Username already exists!");
//         statusLabel->setStyleSheet("color: red;");
//         LOG_WARN("Registration failed: Username already exists - " + username.toStdString());
//         return;
//     }
//
//     // Create new user
//     Core::Entities::User newUser;
//     newUser.createNewUser( username, password );
//     // = ;
//     // newUser.password =; // In real app, hash this!
//     // newUser.id =  // Auto-generate ID
//
//     if (userDao.saveUser(newUser)) {
//         statusLabel->setText("✅ Registration successful! You can now login.");
//         statusLabel->setStyleSheet("color: green;");
//         LOG_INFO("User registered successfully: " + username.toStdString());
//
//         // Clear fields
//         usernameTextField->clear();
//         passwordTextField->clear();
//         confirmPasswordTextField->clear();
//
//         // Show success message
//         QMessageBox::information(this, "Success",
//             "Registration successful!\n\nYou can now login with your credentials.");
//         close();
//     } else {
//         statusLabel->setText("❌ Registration failed! Database error.");
//         statusLabel->setStyleSheet("color: red;");
//         LOG_ERROR("Registration failed for user: " + username.toStdString());
//     }
// }
//
// void registrationScreen::onCancelClicked() {
//     LOG_INFO("Registration cancelled by user");
//     close();
// }
//
// void registrationScreen::onCloseWindow() {
//     LOG_INFO("Registration closed");
// }