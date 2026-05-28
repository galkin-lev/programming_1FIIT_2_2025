//
// Created by lv on 2/3/26.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <memory>

#include "core/model/User.h"

// Forward declarations
class ProfileScreen;
class EditProfileScreen;
class PhotoPickerScreen;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

    void setCurrentUser(const Core::Entities::User& user) { m_currentUser = user; }

private slots:
    void openNewWindow();
    void closeExtraWindow();
    void showMessage();
    // void clearText();
    void onEditProfile();
    void onOpenPhotoPicker();
    void onAvatarUpdated();

private:
    void setupUI();
    void connectSignals();

    // UI Components
    QLabel* windowCountLabel;
    QLineEdit* textField;
    QPushButton* openWindowBtn;
    QPushButton* closeWindowBtn;
    QPushButton* showMessageBtn;
    QPushButton* clearTextBtn;

    // Window management
    int windowCounter = 0;
    QWidget* currentExtraWindow = nullptr;

    // Profile related
    Core::Entities::User m_currentUser;
    ProfileScreen* profileScreen = nullptr;
    EditProfileScreen* editProfileScreen = nullptr;
    PhotoPickerScreen* photoPickerScreen = nullptr;
};

#endif // MAINWINDOW_H