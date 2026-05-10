//
// Created by lv on 4/28/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_DOMAINEXCEPTION_H
#define LAB6_LOGIN_REGISTRATION_DOMAINEXCEPTION_H


#include <stdexcept>
#include <QString>

namespace Core::Entities {

    /**
     * Base exception for all domain-level errors
     */
    class DomainException : public std::runtime_error {
    public:
        explicit DomainException(const QString& message);
        explicit DomainException(const char* message);

        QString getDetailedMessage() const { return m_detailedMessage; }
        void setDetailedMessage(const QString& details) { m_detailedMessage = details; }

    private:
        QString m_detailedMessage;
    };

    /**
     * Thrown when business rule is violated
     */
    class BusinessRuleViolationException : public DomainException {
    public:
        explicit BusinessRuleViolationException(const QString& rule, const QString& details = "");
        QString getRuleName() const { return m_ruleName; }

    private:
        QString m_ruleName;
    };

    /**
     * Thrown when entity validation fails
     */
    class EntityValidationException : public DomainException {
    public:
        explicit EntityValidationException(const QString& entityType, const QString& field, const QString& error);

        QString getEntityType() const { return m_entityType; }
        QString getField() const { return m_field; }
        QString getError() const { return m_error; }

    private:
        QString m_entityType;
        QString m_field;
        QString m_error;
    };

    /**
     * Thrown when entity is not found
     */
    class EntityNotFoundException : public DomainException {
    public:
        explicit EntityNotFoundException(const QString& entityType, qint64 id);
        explicit EntityNotFoundException(const QString& entityType, const QString& identifier);
    };

} // namespace Core::Entities



#endif //LAB6_LOGIN_REGISTRATION_DOMAINEXCEPTION_H