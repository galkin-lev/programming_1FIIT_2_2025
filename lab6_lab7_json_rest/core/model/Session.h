//
// Created by lv on 4/28/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_SESSION_H
#define LAB6_LOGIN_REGISTRATION_SESSION_H


#include <QString>
#include <QDateTime>
#include <QMetaType>
#include "User.h"

namespace Core::Entities {

    /**
     * Session entity - represents a user session
     * Tracks current logged-in user and session state
     */
    class Session {
    public:
        static Session& instance();

        // Session management
        void startSession(const User& user);
        void endSession();
        bool isActive() const { return m_isActive; }

        // Get current user
        User getCurrentUser() const { return m_currentUser; }
        bool hasCurrentUser() const { return m_isActive && m_currentUser.getId() > 0; }

        // Session data
        QString getSessionId() const { return m_sessionId; }
        QDateTime getStartTime() const { return m_startTime; }
        qint64 getSessionDuration() const;

        // Domain rules
        bool isSessionExpired() const;
        void refreshSession();

        // Constants
        static constexpr qint64 SESSION_TIMEOUT_MINUTES = 30;

    private:
        Session() = default;
        Session(const Session&) = delete;
        Session& operator=(const Session&) = delete;

        void generateNewSessionId();

        User m_currentUser;
        QString m_sessionId;
        QDateTime m_startTime;
        QDateTime m_lastActivity;
        bool m_isActive = false;
    };

} // namespace Core::Entities

#endif //LAB6_LOGIN_REGISTRATION_SESSION_H