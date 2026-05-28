//
// Created by lv on 2/3/26.
//

#ifndef LAB3_1_registrationScreen_H
#define LAB3_1_registrationScreen_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QWidget>


#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class registrationScreen : public QMainWindow {
    Q_OBJECT

public:
    explicit registrationScreen(QWidget* parent = nullptr);
    ~registrationScreen();

private slots:
    void onRegisterClicked();
    void onCancelClicked();
    void onCloseWindow();

private:
    void setupUI();
    void connectSignals();

    QLineEdit* usernameTextField;
    QLineEdit* passwordTextField;
    QLineEdit* confirmPasswordTextField;
    QPushButton* registerBtn;
    QPushButton* cancelBtn;
    QLabel* statusLabel;
};

#endif //LAB3_1_registrationScreen_H