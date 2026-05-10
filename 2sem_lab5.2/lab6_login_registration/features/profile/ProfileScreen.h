//
// Created by lv on 5/5/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_PROFILESCREEN_H
#define LAB6_LOGIN_REGISTRATION_PROFILESCREEN_H


#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include "../../core/model/User.h"

class ProfileScreen : public QMainWindow {
    Q_OBJECT

public:
    explicit ProfileScreen(QWidget* parent = nullptr);

    void setUser(const Core::Entities::User& user);
    void refreshAvatar();

    signals:
        void logout();
    void editProfile();  // <-- MAKE SURE THIS LINE EXISTS

private slots:
    void onLogoutClicked();
    void onRefreshClicked();
    void onEditProfileClicked();

private:
    void setupUI();
    void updateDisplay();
    void updateAvatarDisplay();

    Core::Entities::User m_currentUser;

    // UI Components
    QLabel* avatarLabel;
    QLabel* welcomeLabel;
    QLabel* userIdLabel;
    QLabel* sessionIdLabel;
    QLabel* sessionDurationLabel;
    QPushButton* logoutBtn;
    QPushButton* refreshBtn;
    QPushButton* editProfileBtn;
};



#endif //LAB6_LOGIN_REGISTRATION_PROFILESCREEN_H