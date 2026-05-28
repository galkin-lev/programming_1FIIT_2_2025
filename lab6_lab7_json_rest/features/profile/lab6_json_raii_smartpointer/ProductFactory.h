//
// Created by lv on 5/28/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_PRODUCTFACTORY_H
#define LAB6_LOGIN_REGISTRATION_PRODUCTFACTORY_H


#include <memory>
#include "ProductInterface.h"
#include "ProductImpl.h"
#include <QJsonObject>
#include <QDate>

class ProductFactory {
public:
    static std::unique_ptr<ProductInterface> create(const QJsonObject& obj) {
        int id = obj["id"].toInt();
        QString name = obj["name"].toString();
        double price = obj["price"].toDouble();
        QString expDateStr = obj["expiration_date"].toString();

        QDate expDate = QDate::fromString(expDateStr, "yyyy-MM-dd");
        QDate today = QDate::currentDate();

        if (expDate.isValid() && expDate >= today) {
            return std::make_unique<ProductImpl>(id, name, price, expDateStr);
        }
        return nullptr;
    }
};


#endif //LAB6_LOGIN_REGISTRATION_PRODUCTFACTORY_H