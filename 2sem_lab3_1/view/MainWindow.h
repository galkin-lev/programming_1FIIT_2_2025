//
// Created by lv on 2/3/26.
//

#ifndef LAB3_1_MAINWINDOW_H
#define LAB3_1_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void openNewWindow();
    void closeExtraWindow();
    void showMessage();
    void clearText();

private:
    void setupUI();
    void connectSignals();

    QPushButton* openWindowBtn;
    QPushButton* closeWindowBtn;
    QPushButton* showMessageBtn;
    QPushButton* clearTextBtn;
    QLineEdit* textField;
    QLabel* windowCountLabel;
    int windowCounter = 0;
    QWidget* currentExtraWindow = nullptr;
};


#endif //LAB3_1_MAINWINDOW_H