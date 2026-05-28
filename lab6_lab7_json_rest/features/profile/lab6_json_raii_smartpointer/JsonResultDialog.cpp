//
// Created by lv on 5/28/26.
//

#include "JsonResultDialog.h"

#include "JsonResultDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QDate>

JsonResultDialog::JsonResultDialog(const ProcessingResult& result, QWidget* parent)
    : QDialog(parent) {

    setWindowTitle("JSON Warehouse Result (Variant 2)");
    resize(650, 500);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QTextEdit* outputArea = new QTextEdit(this);
    outputArea->setReadOnly(true);
    outputArea->setFont(QFont("Monospace", 10));

    QString text;

    if (result.success) {
        text = "РЕЗУЛЬТАТ ОБРАБОТКИ (Вариант 2)\n";
        text += "================================\n\n";
        text += QString("Текущая дата: %1\n").arg(QDate::currentDate().toString("yyyy-MM-dd"));
        text += "-----------------------------------------------\n";
        text += "ID | Name (измененный) | Price | Expiration Date\n";
        text += "-----------------------------------------------\n";

        for (const auto& product : result.products) {
            text += product->toString() + "\n";
        }

        text += "\n-----------------------------------------------\n";
        text += QString("✅ Обработано свежих товаров: %1\n").arg(result.processedCount);
        text += "✅ Результат сохранен в: " + result.outputFile;
    } else {
        text = "❌ ОШИБКА ОБРАБОТКИ\n\n";
        text += result.errorMessage;
    }

    outputArea->setPlainText(text);

    QPushButton* closeBtn = new QPushButton("Close", this);
    closeBtn->setFixedWidth(100);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);
    buttonLayout->addStretch();

    mainLayout->addWidget(outputArea);
    mainLayout->addLayout(buttonLayout);

    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
}