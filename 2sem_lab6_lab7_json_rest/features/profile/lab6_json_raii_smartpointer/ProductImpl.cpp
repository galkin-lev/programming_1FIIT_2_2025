//
// Created by lv on 5/28/26.
//

#include "ProductImpl.h"
#include "ProductImpl.h"
#include <QDate>
#include <QChar>

// ============ VAR 2 ===========



ProductImpl::ProductImpl(int id, const QString& name, double price, const QString& expDate)
    : m_id(id)
    , m_originalName(name)
    , m_price(price)
    , m_expDate(expDate) {
    m_modifiedName = convertVowelsToUpper(name);
}

QString ProductImpl::convertVowelsToUpper(const QString& name) {
    QString result = name;

    for (int i = 0; i < result.length(); ++i) {
        QChar ch = result[i];
        QChar upperCh = ch;

        // Русские гласные (сравниваем как QChar, а не как char)
        if (ch == QChar('а') || ch == QChar('е') || ch == QChar('ё') ||
            ch == QChar('и') || ch == QChar('о') || ch == QChar('у') ||
            ch == QChar('ы') || ch == QChar('э') || ch == QChar('ю') ||
            ch == QChar('я')) {
            upperCh = ch.toUpper();
            }
        // Английские гласные
        else if (ch == QChar('a') || ch == QChar('e') || ch == QChar('i') ||
                 ch == QChar('o') || ch == QChar('u')) {
            upperCh = ch.toUpper();
                 }

        result[i] = upperCh;
    }

    return result;
}

QJsonObject ProductImpl::toJson() const {
    QJsonObject obj;
    obj["id"] = m_id;
    obj["name"] = m_modifiedName;
    obj["price"] = m_price;
    obj["expiration_date"] = m_expDate;
    return obj;
}

QString ProductImpl::toString() const {
    return QString("%1 | %2 | %3 | %4")
        .arg(m_id, 2)
        .arg(m_modifiedName, -25)
        .arg(m_price, 8, 'f', 2)
        .arg(m_expDate);
}