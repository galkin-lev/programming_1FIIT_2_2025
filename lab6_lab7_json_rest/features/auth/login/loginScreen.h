//
// Created by lv on 2/3/26.
//

#ifndef LAB3_1_loginScreen_H
#define LAB3_1_loginScreen_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QWidget>
#include "../registration/registrationScreen.h"  // Add this
#include "../../profile/ProfileScreen.h"

class registrationScreen;

class loginScreen : public QMainWindow {
    Q_OBJECT

public:
    loginScreen(QWidget* parent = nullptr);

private slots:
    void openRegistration();
    void openNewWindow();
    void closeExtraWindow();
    void login();
    void clearText();
    void openProfileScreen();
    void onLogout();

private:
    void setupUI();
    void connectSignals();
    ProfileScreen* profileScreen = nullptr;


    registrationScreen* registrationWindow = nullptr;

    QPushButton* registerBtn;
    // QPushButton* closeWindowBtn;
    QPushButton* loginBtn;
    QPushButton* clearTextBtn;
    QLineEdit* usernameTextField;
    QLineEdit* passwordTextField;
    QLabel* windowCountLabel;
    int windowCounter = 0;
    QWidget* currentExtraWindow = nullptr;
};


#endif //LAB3_1_loginScreen_H