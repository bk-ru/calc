#ifndef DISPLAYFORMATTER_H
#define DISPLAYFORMATTER_H

#include <QString>

// Класс для форматирования отображения чисел
class DisplayFormatter
{
public:
    DisplayFormatter() = default;
    ~DisplayFormatter() = default;

    static QString formatNumber(double value, int maxDigits = 10);
    static bool isValidNumber(const QString& text);
    static double toDouble(const QString& text, bool* ok = nullptr);
    static QString removeTrailingDecimal(const QString& text);
    static bool hasDecimalPoint(const QString& text);
};

#endif // DISPLAYFORMATTER_H
