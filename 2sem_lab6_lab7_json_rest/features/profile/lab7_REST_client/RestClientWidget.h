//
// Created by lv on 5/28/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_RESTCLIENTWIDGET_H
#define LAB6_LOGIN_REGISTRATION_RESTCLIENTWIDGET_H



#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class RestClientWidget : public QWidget {
    Q_OBJECT

public:
    explicit RestClientWidget(QWidget* parent = nullptr);

private slots:
    void onFetchJokeClicked();
    void onReplyFinished(QNetworkReply* reply);

private:
    void setupUI();
    void displayJoke(const QString& joke);
    void displayError(const QString& error);
    void setLoading(bool loading);
    void onCloseClicked();


    QNetworkAccessManager m_manager;
    QPushButton* m_fetchBtn;
    QTextEdit* m_outputArea;
    QLabel* m_statusLabel;
    QPushButton* m_closeBtn;
};



#endif //LAB6_LOGIN_REGISTRATION_RESTCLIENTWIDGET_H