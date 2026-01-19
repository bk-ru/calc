#include "calchandler.h"
#include <cmath>
#include <QDebug>

CalcHandler::CalcHandler(QObject *parent)
    : QObject(parent)
    , m_state(State::Idle)
    , m_storedValue(0.0)
    , m_operation(Operation::None)
    , m_hasStoredValue(false)
{
}

void CalcHandler::setOperand(double value)
{
    m_storedValue = value;
    m_hasStoredValue = true;
    
    if (m_state == State::Idle || m_state == State::ResultDisplayed) {
        m_state = State::OperandEntry;
    }
}

void CalcHandler::setOperation(Operation op)
{
    if (!m_hasStoredValue) {
        return;
    }
    
    m_operation = op;
    m_state = State::OperatorSelected;
}

CalcHandler::CalculationResult CalcHandler::calculate()
{
    if (!m_hasStoredValue || m_operation == Operation::None) {
        return {false, 0.0, "Недостаточно данных для вычисления"};
    }
    
    m_state = State::ResultDisplayed;
    return {true, m_storedValue, ""};
}

CalcHandler::CalculationResult CalcHandler::performBinaryOperation(
    double operand1, double operand2, Operation op)
{
    CalculationResult result;
    result.success = true;
    result.errorMessage = "";
    
    switch (op) {
        case Operation::Add:
            result.value = operand1 + operand2;
            break;
            
        case Operation::Subtract:
            result.value = operand1 - operand2;
            break;
            
        case Operation::Multiply:
            result.value = operand1 * operand2;
            break;
            
        case Operation::Divide:
            if (!isValidDivision(operand2)) {
                result.success = false;
                result.value = 0.0;
                result.errorMessage = "Ошибка: деление на 0";
                m_state = State::Error;
                return result;
            }
            result.value = operand1 / operand2;
            break;
            
        default:
            result.success = false;
            result.value = 0.0;
            result.errorMessage = "Неизвестная операция";
            m_state = State::Error;
            return result;
    }
    
    m_storedValue = result.value;
    m_state = State::ResultDisplayed;
    return result;
}

CalcHandler::CalculationResult CalcHandler::applyUnaryOperation(Operation op, double value)
{
    CalculationResult result;
    result.success = true;
    result.errorMessage = "";
    
    switch (op) {
        case Operation::Percent:
            result.value = value * 0.01;
            break;
            
        case Operation::Negate:
            result.value = -value;
            break;
            
        case Operation::Square:
            result.value = value * value;
            break;
            
        case Operation::SquareRoot:
            if (value < 0.0) {
                result.success = false;
                result.value = 0.0;
                result.errorMessage = "Ошибка: корень из отрицательного числа";
                m_state = State::Error;
                return result;
            }
            result.value = std::sqrt(value);
            break;
            
        case Operation::Reciprocal:
            if (!isValidDivision(value)) {
                result.success = false;
                result.value = 0.0;
                result.errorMessage = "Ошибка: деление на 0";
                m_state = State::Error;
                return result;
            }
            result.value = 1.0 / value;
            break;
            
        default:
            result.success = false;
            result.value = 0.0;
            result.errorMessage = "Неизвестная унарная операция";
            return result;
    }
    
    return result;
}

void CalcHandler::clear()
{
    m_storedValue = 0.0;
    m_operation = Operation::None;
    m_hasStoredValue = false;
    m_state = State::Idle;
}

void CalcHandler::reset()
{
    clear();
}

CalcHandler::State CalcHandler::currentState() const
{
    return m_state;
}

double CalcHandler::storedValue() const
{
    return m_storedValue;
}

CalcHandler::Operation CalcHandler::currentOperation() const
{
    return m_operation;
}

bool CalcHandler::hasStoredValue() const
{
    return m_hasStoredValue;
}

CalcHandler::Operation CalcHandler::operationFromChar(QChar c)
{
    if (c == '+') return Operation::Add;
    if (c == '-') return Operation::Subtract;
    if (c == 'x' || c == 'X' || c == '*' || c == QChar(0x00D7)) return Operation::Multiply;  // × = U+00D7
    if (c == '/' || c == QChar(0x00F7)) return Operation::Divide;  // ÷ = U+00F7
    if (c == '%') return Operation::Percent;
    
    return Operation::None;
}

QString CalcHandler::operationToString(Operation op)
{
    switch (op) {
        case Operation::Add: return "+";
        case Operation::Subtract: return "-";
        case Operation::Multiply: return "×";
        case Operation::Divide: return "÷";
        case Operation::Percent: return "%";
        case Operation::Negate: return "±";
        case Operation::Square: return "x²";
        case Operation::SquareRoot: return "√";
        case Operation::Reciprocal: return "1/x";
        default: return "";
    }
}

bool CalcHandler::isValidDivision(double divisor) const
{
    return !qFuzzyCompare(divisor, 0.0);
}
