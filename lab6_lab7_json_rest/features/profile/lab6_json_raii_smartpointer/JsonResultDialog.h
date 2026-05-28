//
// Created by lv on 5/28/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_JSONRESULTDIALOG_H
#define LAB6_LOGIN_REGISTRATION_JSONRESULTDIALOG_H



#include <QDialog>
#include "facade/JsonProcessorFacade.h"

class JsonResultDialog : public QDialog {
    Q_OBJECT

public:
    explicit JsonResultDialog(const ProcessingResult& result, QWidget* parent = nullptr);
};


#endif //LAB6_LOGIN_REGISTRATION_JSONRESULTDIALOG_H