//
// Created by lv on 4/28/26.
//

#include "DomainException.h"

namespace Core::Entities {

    DomainException::DomainException(const QString& message)
        : std::runtime_error(message.toStdString()) {}

    DomainException::DomainException(const char* message)
        : std::runtime_error(message) {}

    BusinessRuleViolationException::BusinessRuleViolationException(const QString& rule, const QString& details)
        : DomainException("Business rule violation: " + rule), m_ruleName(rule) {
        setDetailedMessage(details);
    }

    EntityValidationException::EntityValidationException(const QString& entityType,
                                                          const QString& field,
                                                          const QString& error)
        : DomainException(QString("Validation failed for %1.%2: %3")
                         .arg(entityType, field, error))
        , m_entityType(entityType), m_field(field), m_error(error) {}

    EntityNotFoundException::EntityNotFoundException(const QString& entityType, qint64 id)
        : DomainException(QString("%1 with id %2 not found").arg(entityType).arg(id)) {}

    EntityNotFoundException::EntityNotFoundException(const QString& entityType, const QString& identifier)
        : DomainException(QString("%1 '%2' not found").arg(entityType, identifier)) {}

}