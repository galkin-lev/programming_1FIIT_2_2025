//
// Created by lv on 5/28/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_JSONFILEREADER_H
#define LAB6_LOGIN_REGISTRATION_JSONFILEREADER_H


#include <QFile>
#include <QByteArray>
#include "../../../logger/logger.h"

class JsonFileReader {
    QFile m_file;
    bool m_isOpen;

public:
    JsonFileReader(const QString& path) : m_file(path) {
        m_isOpen = m_file.open(QIODevice::ReadOnly);
        if (!m_isOpen) {
            LOG_ERROR("Failed to open file: " + path.toStdString());
        } else {
            LOG_DEBUG("File opened: " + path.toStdString());
        }
    }

    ~JsonFileReader() {
        if (m_isOpen && m_file.isOpen()) {
            m_file.close();
            LOG_DEBUG("File closed: " + m_file.fileName().toStdString());
        }
    }

    QByteArray readAll() {
        if (!m_isOpen) return QByteArray();
        return m_file.readAll();
    }

    bool isOpen() const { return m_isOpen; }

    // Запрещаем копирование
    JsonFileReader(const JsonFileReader&) = delete;
    JsonFileReader& operator=(const JsonFileReader&) = delete;
};


#endif //LAB6_LOGIN_REGISTRATION_JSONFILEREADER_H