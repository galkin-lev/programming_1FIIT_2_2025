//
// Created by lv on 5/28/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_PRODUCTINTERFACE_H
#define LAB6_LOGIN_REGISTRATION_PRODUCTINTERFACE_H

#include <QJsonObject>
#include <QString>

class ProductInterface {
public:
    virtual ~ProductInterface() = default;

    virtual QJsonObject toJson() const = 0;
    virtual QString toString() const = 0;
    virtual double getPrice() const = 0;
    virtual int getId() const = 0;
    virtual QString getExpirationDate() const = 0;
    virtual QString getDisplayName() const = 0;
};


#endif //LAB6_LOGIN_REGISTRATION_PRODUCTINTERFACE_H