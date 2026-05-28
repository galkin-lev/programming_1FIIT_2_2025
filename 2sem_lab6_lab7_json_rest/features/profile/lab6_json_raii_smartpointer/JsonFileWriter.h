//
// Created by lv on 5/28/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_JSONFILEWRITER_H
#define LAB6_LOGIN_REGISTRATION_JSONFILEWRITER_H


#include <QFile>
#include <QByteArray>
#include "../../../logger/logger.h"



class JsonFileWriter {
    QFile m_file;
    bool m_isOpen;

public:
    JsonFileWriter(const QString& path) : m_file(path) {
        m_isOpen = m_file.open(QIODevice::WriteOnly);
        if (!m_isOpen) {
            LOG_ERROR("Failed to open file for writing: " + path.toStdString());
        }
    }

    ~JsonFileWriter() {
        if (m_isOpen && m_file.isOpen()) {
            m_file.close();
            LOG_DEBUG("File closed: " + m_file.fileName().toStdString());
        }
    }

    bool write(const QByteArray& data) {
        if (!m_isOpen) return false;
        return m_file.write(data) != -1;
    }

    bool isOpen() const { return m_isOpen; }

    JsonFileWriter(const JsonFileWriter&) = delete;
    JsonFileWriter& operator=(const JsonFileWriter&) = delete;
};

#endif //LAB6_LOGIN_REGISTRATION_JSONFILEWRITER_H