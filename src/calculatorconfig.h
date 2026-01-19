#ifndef CALCULATORCONFIG_H
#define CALCULATORCONFIG_H

#include <QString>

// Конфигурация калькулятора - централизованное хранение констант
namespace CalculatorConfig {
    constexpr int MAX_DIGIT_LENGTH = 10;
    constexpr int MAX_DECIMAL_PLACES = 10;
    constexpr char NUMBER_FORMAT = 'g';
    constexpr int PRECISION = 10;
    
    const QString ERROR_DIVISION_BY_ZERO = "Ошибка: деление на 0";
    const QString ERROR_INVALID_INPUT = "Ошибка: неверный ввод";
    const QString ERROR_OVERFLOW = "Ошибка: переполнение";
    const QString ERROR_SQRT_NEGATIVE = "Ошибка: корень из отрицательного числа";
    
    const QString DECIMAL_SEPARATOR = ".";
    const QString ZERO_WITH_DECIMAL = "0.";
}

#endif // CALCULATORCONFIG_H
