#ifndef CALCHANDLER_H
#define CALCHANDLER_H

#include <QObject>
#include <QString>
#include <QChar>

// Класс для обработки бизнес-логики калькулятора
// Отвечает за выполнение арифметических операций, управление состоянием
// калькулятора и валидацию операций. Не содержит UI-логики.
class CalcHandler : public QObject
{
    Q_OBJECT

public:
    enum class State {
        Idle,
        OperandEntry,
        OperatorSelected,
        ResultDisplayed,
        Error
    };

    enum class Operation {
        None,
        Add,
        Subtract,
        Multiply,
        Divide,
        Percent,
        Negate,
        Square,
        SquareRoot,
        Reciprocal
    };

    struct CalculationResult {
        bool success;
        double value;
        QString errorMessage;
    };

    explicit CalcHandler(QObject *parent = nullptr);
    ~CalcHandler() override = default;

    void setOperand(double value);
    void setOperation(Operation op);
    CalculationResult calculate();
    CalculationResult applyUnaryOperation(Operation op, double value);
    CalculationResult performBinaryOperation(double operand1, double operand2, Operation op);
    
    void clear();
    void reset();
    State currentState() const;
    
    double storedValue() const;
    Operation currentOperation() const;
    bool hasStoredValue() const;

    static Operation operationFromChar(QChar c);
    static QString operationToString(Operation op);

private:
    State m_state;
    double m_storedValue;
    Operation m_operation;
    bool m_hasStoredValue;
    
    bool isValidDivision(double divisor) const;
};

#endif // CALCHANDLER_H
