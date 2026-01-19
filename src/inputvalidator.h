#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <QString>

// Класс для валидации пользовательского ввода
class InputValidator
{
public:
    InputValidator() = default;
    ~InputValidator() = default;

    static bool canAddDigit(const QString& currentText, int maxLength);
    static bool canAddDecimalPoint(const QString& currentText, int maxLength);
    static bool isNotEmpty(const QString& text);
    static bool isLengthExceeded(const QString& text, int maxLength);
};

#endif // INPUTVALIDATOR_H
