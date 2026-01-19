#include "inputvalidator.h"
#include "displayformatter.h"

bool InputValidator::canAddDigit(const QString& currentText, int maxLength)
{
    return currentText.length() < maxLength;
}

bool InputValidator::canAddDecimalPoint(const QString& currentText, int maxLength)
{
    if (DisplayFormatter::hasDecimalPoint(currentText)) {
        return false;
    }
    
    if (currentText.length() >= maxLength - 1) {
        return false;
    }
    
    return true;
}

bool InputValidator::isNotEmpty(const QString& text)
{
    return !text.isEmpty();
}

bool InputValidator::isLengthExceeded(const QString& text, int maxLength)
{
    return text.length() >= maxLength;
}
