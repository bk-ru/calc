#ifndef CALCHANDLER_H
#define CALCHANDLER_H

#include <QObject>
#include <QString>
#include <QChar>

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

public:
    explicit CalcHandler(QObject *parent = nullptr);
    ~CalcHandler() override = default;

public:
    void setOperand(double value);
    void setOperation(Operation op);
    CalculationResult calculate();
    CalculationResult applyUnaryOperation(Operation op, double value);
    CalculationResult performBinaryOperation(double operand1, double operand2, Operation op);
    
public:
    void clear();
    void reset();
    State currentState() const;

public:
    double storedValue() const;
    Operation currentOperation() const;
    bool hasStoredValue() const;

public:
    static Operation operationFromChar(QChar c);
    static QString operationToString(Operation op);

private:
    bool isValidDivision(double divisor) const;

private:
    State m_state;
    double m_storedValue;
    Operation m_operation;
    bool m_hasStoredValue;
};

#endif // CALCHANDLER_H
