//
// Created by lv on 5/28/26.
//

#include "RestClientWidget.h"

#include "RestClientWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonObject>
RestClientWidget::RestClientWidget(QWidget* parent) : QWidget(parent) {
    setupUI();
    connect(&m_manager, &QNetworkAccessManager::finished,
            this, &RestClientWidget::onReplyFinished);
}

void RestClientWidget::setupUI() {
    // Делаем окно плавающим (Qt::Window)
    setWindowFlags(Qt::Window);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Заголовок
    QLabel* titleLabel = new QLabel("😂 Random Joke Generator", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // Кнопка получения шутки
    m_fetchBtn = new QPushButton("🎲 Get Random Joke", this);
    m_fetchBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #4CAF50;"
        "  color: white;"
        "  padding: 10px;"
        "  font-size: 14px;"
        "  border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #45a049;"
        "}"
    );

    // Область вывода (чёрный текст)
    m_outputArea = new QTextEdit(this);
    m_outputArea->setReadOnly(true);
    m_outputArea->setPlaceholderText("Click the button to get a random joke...");
    m_outputArea->setMinimumHeight(200);
    m_outputArea->setStyleSheet(
        "QTextEdit {"
        "  font-family: 'Segoe UI', Arial;"
        "  font-size: 14px;"
        "  color: black;"           // ← ЧЁРНЫЙ ТЕКСТ
        "  background-color: #f5f5f5;"
        "  border: 1px solid #ddd;"
        "  border-radius: 5px;"
        "  padding: 10px;"
        "}"
    );

    // Статус
    m_statusLabel = new QLabel("Ready", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);

    // Кнопка закрытия (внизу)
    m_closeBtn = new QPushButton("✖ Close", this);
    m_closeBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #f44336;"
        "  color: white;"
        "  padding: 8px;"
        "  font-size: 12px;"
        "  border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #D32F2F;"
        "}"
    );

    // Добавляем элементы в layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_fetchBtn);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_outputArea);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_closeBtn);

    setWindowTitle("REST Client - Random Jokes");
    setMinimumSize(500, 450);

    // Connect сигналов
    connect(m_fetchBtn, &QPushButton::clicked, this, &RestClientWidget::onFetchJokeClicked);
    connect(m_closeBtn, &QPushButton::clicked, this, &RestClientWidget::onCloseClicked);
}

void RestClientWidget::onFetchJokeClicked() {
    setLoading(true);

    QUrl url("https://v2.jokeapi.dev/joke/Any?format=json");
    QNetworkRequest request(url);
    m_manager.get(request);
}

void RestClientWidget::onReplyFinished(QNetworkReply* reply) {
    setLoading(false);

    if (reply->error() != QNetworkReply::NoError) {
        displayError(reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull()) {
        displayError("Invalid JSON response");
        reply->deleteLater();
        return;
    }

    QJsonObject obj = doc.object();

    QString joke;
    if (obj.contains("joke")) {
        joke = obj["joke"].toString();
    } else if (obj.contains("setup") && obj.contains("delivery")) {
        joke = obj["setup"].toString() + "\n\n" + obj["delivery"].toString();
    } else {
        displayError("Unexpected API response format");
        reply->deleteLater();
        return;
    }

    displayJoke(joke);
    reply->deleteLater();
}

void RestClientWidget::displayJoke(const QString& joke) {
    QString output;
    output += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    output += "🎭 RANDOM JOKE 🎭\n";
    output += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    output += joke;

    m_outputArea->setPlainText(output);
    m_statusLabel->setText("✅ Joke loaded!");
}

void RestClientWidget::displayError(const QString& error) {
    m_outputArea->setPlainText(
        "❌ ERROR ❌\n\n"
        "Could not fetch joke.\n"
        "Error: " + error + "\n\n"
        "Check your internet connection and try again."
    );
    m_statusLabel->setText("❌ Failed to load joke");
    m_statusLabel->setStyleSheet("color: red;");
}

void RestClientWidget::setLoading(bool loading) {
    m_fetchBtn->setEnabled(!loading);
    if (loading) {
        m_statusLabel->setText("⏳ Loading...");
        m_statusLabel->setStyleSheet("color: blue;");
        m_outputArea->setPlainText("Fetching a joke from the internet...");
    } else {
        m_statusLabel->setStyleSheet("");
    }
}

void RestClientWidget::onCloseClicked() {
    this->close();
}