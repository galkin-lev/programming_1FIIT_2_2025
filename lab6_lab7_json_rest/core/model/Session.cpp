// core/entities/Session.cpp
#include "Session.h"
#include <QUuid>

namespace Core::Entities {

    Session& Session::instance() {
        static Session instance;
        return instance;
    }

    void Session::startSession(const User& user) {
        m_currentUser = user;
        generateNewSessionId();
        m_startTime = QDateTime::currentDateTime();
        m_lastActivity = m_startTime;
        m_isActive = true;
    }

    void Session::endSession() {
        m_currentUser = User();
        m_sessionId.clear();
        m_startTime = QDateTime();
        m_lastActivity = QDateTime();
        m_isActive = false;
    }

    qint64 Session::getSessionDuration() const {
        if (!m_isActive) return 0;
        return m_startTime.secsTo(QDateTime::currentDateTime());
    }

    bool Session::isSessionExpired() const {
        if (!m_isActive) return true;
        qint64 inactiveMinutes = m_lastActivity.secsTo(QDateTime::currentDateTime()) / 60;
        return inactiveMinutes > SESSION_TIMEOUT_MINUTES;
    }

    void Session::refreshSession() {
        if (m_isActive) {
            m_lastActivity = QDateTime::currentDateTime();
        }
    }

    void Session::generateNewSessionId() {
        m_sessionId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }

} // namespace Core::Entities