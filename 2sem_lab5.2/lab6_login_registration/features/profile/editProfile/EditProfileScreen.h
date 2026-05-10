//
// Created by lv on 5/10/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_EDITPROFILESCREEN_H
#define LAB6_LOGIN_REGISTRATION_EDITPROFILESCREEN_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "core/model/User.h"

class EditProfileScreen : public QMainWindow {
    Q_OBJECT

public:
    explicit EditProfileScreen(QWidget* parent = nullptr);

    void setUser(const Core::Entities::User& user);

    signals:
        void profileUpdated();
    void openPhotoPicker();

private slots:
    void onSaveClicked();
    void onCancelClicked();
    void onManagePhotosClicked();

private:
    void setupUI();
    bool validateInputs();
    void loadUserData();

    // UI Components
    QLineEdit* m_usernameEdit;
    QLineEdit* m_emailEdit;
    QLineEdit* m_currentPasswordEdit;
    QLineEdit* m_newPasswordEdit;
    QLineEdit* m_confirmPasswordEdit;
    QPushButton* m_saveBtn;
    QPushButton* m_cancelBtn;
    QPushButton* m_managePhotosBtn;
    QLabel* m_errorLabel;

    // Data
    Core::Entities::User m_currentUser;
};



#endif //LAB6_LOGIN_REGISTRATION_EDITPROFILESCREEN_H