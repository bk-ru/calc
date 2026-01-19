#include "displayformatter.h"
#include "calculatorconfig.h"
#include <QLocale>

QString DisplayFormatter::formatNumber(double value, int maxDigits)
{
    return QString::number(value, CalculatorConfig::NUMBER_FORMAT, maxDigits);
}

bool DisplayFormatter::isValidNumber(const QString& text)
{
    if (text.isEmpty()) {
        return false;
    }
    
    bool ok = false;
    text.toDouble(&ok);
    return ok;
}

double DisplayFormatter::toDouble(const QString& text, bool* ok)
{
    return text.toDouble(ok);
}

QString DisplayFormatter::removeTrailingDecimal(const QString& text)
{
    if (text.endsWith(CalculatorConfig::DECIMAL_SEPARATOR)) {
        QString result = text;
        result.chop(1);
        return result;
    }
    return text;
}

bool DisplayFormatter::hasDecimalPoint(const QString& text)
{
    return text.contains(CalculatorConfig::DECIMAL_SEPARATOR);
}
