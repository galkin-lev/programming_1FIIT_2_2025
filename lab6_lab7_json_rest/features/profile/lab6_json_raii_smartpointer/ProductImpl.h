//
// Created by lv on 5/28/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_PRODUCTIMPL_H
#define LAB6_LOGIN_REGISTRATION_PRODUCTIMPL_H


#include "ProductInterface.h"
#include <QString>
#include <QJsonObject>

class ProductImpl : public ProductInterface {
public:
    ProductImpl(int id, const QString& name, double price, const QString& expDate);

    QJsonObject toJson() const override;
    QString toString() const override;
    double getPrice() const override { return m_price; }
    int getId() const override { return m_id; }
    QString getExpirationDate() const override { return m_expDate; }
    QString getDisplayName() const override { return m_modifiedName; }

private:
    static QString convertVowelsToUpper(const QString& name);

    int m_id;
    QString m_originalName;
    QString m_modifiedName;
    double m_price;
    QString m_expDate;
};



#endif //LAB6_LOGIN_REGISTRATION_PRODUCTIMPL_H