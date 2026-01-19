#include "calchandler.h"
#include <QtTest/QtTest>

/**
 * @brief Тесты для класса CalcHandler
 * 
 * Проверяет корректность бизнес-логики калькулятора:
 * - Базовые арифметические операции
 * - Унарные операции
 * - Обработку ошибок
 * - Управление состоянием
 */
class TestCalcHandler : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    // Тесты базовых операций
    void testAddition();
    void testSubtraction();
    void testMultiplication();
    void testDivision();
    void testDivisionByZero();
    
    // Тесты унарных операций
    void testPercent();
    void testNegate();
    void testSquare();
    void testSquareRoot();
    void testSquareRootNegative();
    void testReciprocal();
    void testReciprocalZero();
    
    // Тесты управления состоянием
    void testInitialState();
    void testClear();
    void testStateTransitions();
    
    // Тесты утилит
    void testOperationFromChar();
    void testOperationToString();
    
    // Тесты граничных случаев
    void testLargeNumbers();
    void testSmallNumbers();
    void testChainedOperations();

private:
    CalcHandler *m_handler;
};

void TestCalcHandler::initTestCase()
{
    // Выполняется один раз перед всеми тестами
}

void TestCalcHandler::cleanupTestCase()
{
    // Выполняется один раз после всех тестов
}

void TestCalcHandler::init()
{
    // Выполняется перед каждым тестом
    m_handler = new CalcHandler();
}

void TestCalcHandler::cleanup()
{
    // Выполняется после каждого теста
    delete m_handler;
    m_handler = nullptr;
}

// ============================================================
// ТЕСТЫ БАЗОВЫХ ОПЕРАЦИЙ
// ============================================================

void TestCalcHandler::testAddition()
{
    auto result = m_handler->performBinaryOperation(5.0, 3.0, CalcHandler::Operation::Add);
    
    QVERIFY(result.success);
    QCOMPARE(result.value, 8.0);
    QVERIFY(result.errorMessage.isEmpty());
}

void TestCalcHandler::testSubtraction()
{
    auto result = m_handler->performBinaryOperation(10.0, 4.0, CalcHandler::Operation::Subtract);
    
    QVERIFY(result.success);
    QCOMPARE(result.value, 6.0);
}

void TestCalcHandler::testMultiplication()
{
    auto result = m_handler->performBinaryOperation(6.0, 7.0, CalcHandler::Operation::Multiply);
    
    QVERIFY(result.success);
    QCOMPARE(result.value, 42.0);
}

void TestCalcHandler::testDivision()
{
    auto result = m_handler->performBinaryOperation(15.0, 3.0, CalcHandler::Operation::Divide);
    
    QVERIFY(result.success);
    QCOMPARE(result.value, 5.0);
}

void TestCalcHandler::testDivisionByZero()
{
    auto result = m_handler->performBinaryOperation(10.0, 0.0, CalcHandler::Operation::Divide);
    
    QVERIFY(!result.success);
    QCOMPARE(result.value, 0.0);
    QVERIFY(!result.errorMessage.isEmpty());
    QCOMPARE(m_handler->currentState(), CalcHandler::State::Error);
}

// ============================================================
// ТЕСТЫ УНАРНЫХ ОПЕРАЦИЙ
// ============================================================

void TestCalcHandler::testPercent()
{
    auto result = m_handler->applyUnaryOperation(CalcHandler::Operation::Percent, 50.0);
    
    QVERIFY(result.success);
    QCOMPARE(result.value, 0.5);
}

void TestCalcHandler::testNegate()
{
    auto result = m_handler->applyUnaryOperation(CalcHandler::Operation::Negate, 42.0);
    QVERIFY(result.success);
    QCOMPARE(result.value, -42.0);
    
    result = m_handler->applyUnaryOperation(CalcHandler::Operation::Negate, -42.0);
    QVERIFY(result.success);
    QCOMPARE(result.value, 42.0);
}

void TestCalcHandler::testSquare()
{
    auto result = m_handler->applyUnaryOperation(CalcHandler::Operation::Square, 5.0);
    
    QVERIFY(result.success);
    QCOMPARE(result.value, 25.0);
}

void TestCalcHandler::testSquareRoot()
{
    auto result = m_handler->applyUnaryOperation(CalcHandler::Operation::SquareRoot, 16.0);
    
    QVERIFY(result.success);
    QCOMPARE(result.value, 4.0);
}

void TestCalcHandler::testSquareRootNegative()
{
    auto result = m_handler->applyUnaryOperation(CalcHandler::Operation::SquareRoot, -4.0);
    
    QVERIFY(!result.success);
    QVERIFY(!result.errorMessage.isEmpty());
    QCOMPARE(m_handler->currentState(), CalcHandler::State::Error);
}

void TestCalcHandler::testReciprocal()
{
    auto result = m_handler->applyUnaryOperation(CalcHandler::Operation::Reciprocal, 4.0);
    
    QVERIFY(result.success);
    QCOMPARE(result.value, 0.25);
}

void TestCalcHandler::testReciprocalZero()
{
    auto result = m_handler->applyUnaryOperation(CalcHandler::Operation::Reciprocal, 0.0);
    
    QVERIFY(!result.success);
    QVERIFY(!result.errorMessage.isEmpty());
    QCOMPARE(m_handler->currentState(), CalcHandler::State::Error);
}

// ============================================================
// ТЕСТЫ УПРАВЛЕНИЯ СОСТОЯНИЕМ
// ============================================================

void TestCalcHandler::testInitialState()
{
    QCOMPARE(m_handler->currentState(), CalcHandler::State::Idle);
    QVERIFY(!m_handler->hasStoredValue());
    QCOMPARE(m_handler->storedValue(), 0.0);
    QCOMPARE(m_handler->currentOperation(), CalcHandler::Operation::None);
}

void TestCalcHandler::testClear()
{
    m_handler->setOperand(42.0);
    m_handler->setOperation(CalcHandler::Operation::Add);
    
    m_handler->clear();
    
    QCOMPARE(m_handler->currentState(), CalcHandler::State::Idle);
    QVERIFY(!m_handler->hasStoredValue());
    QCOMPARE(m_handler->storedValue(), 0.0);
    QCOMPARE(m_handler->currentOperation(), CalcHandler::Operation::None);
}

void TestCalcHandler::testStateTransitions()
{
    // Idle -> OperandEntry
    m_handler->setOperand(5.0);
    QCOMPARE(m_handler->currentState(), CalcHandler::State::OperandEntry);
    
    // OperandEntry -> OperatorSelected
    m_handler->setOperation(CalcHandler::Operation::Add);
    QCOMPARE(m_handler->currentState(), CalcHandler::State::OperatorSelected);
    
    // OperatorSelected -> ResultDisplayed
    m_handler->performBinaryOperation(5.0, 3.0, CalcHandler::Operation::Add);
    QCOMPARE(m_handler->currentState(), CalcHandler::State::ResultDisplayed);
}

// ============================================================
// ТЕСТЫ УТИЛИТ
// ============================================================

void TestCalcHandler::testOperationFromChar()
{
    QCOMPARE(CalcHandler::operationFromChar('+'), CalcHandler::Operation::Add);
    QCOMPARE(CalcHandler::operationFromChar('-'), CalcHandler::Operation::Subtract);
    QCOMPARE(CalcHandler::operationFromChar('*'), CalcHandler::Operation::Multiply);
    QCOMPARE(CalcHandler::operationFromChar('x'), CalcHandler::Operation::Multiply);
    QCOMPARE(CalcHandler::operationFromChar('/'), CalcHandler::Operation::Divide);
    QCOMPARE(CalcHandler::operationFromChar('%'), CalcHandler::Operation::Percent);
    QCOMPARE(CalcHandler::operationFromChar('?'), CalcHandler::Operation::None);
}

void TestCalcHandler::testOperationToString()
{
    QCOMPARE(CalcHandler::operationToString(CalcHandler::Operation::Add), "+");
    QCOMPARE(CalcHandler::operationToString(CalcHandler::Operation::Subtract), "-");
    QCOMPARE(CalcHandler::operationToString(CalcHandler::Operation::Multiply), "×");
    QCOMPARE(CalcHandler::operationToString(CalcHandler::Operation::Divide), "÷");
}

// ============================================================
// ТЕСТЫ ГРАНИЧНЫХ СЛУЧАЕВ
// ============================================================

void TestCalcHandler::testLargeNumbers()
{
    auto result = m_handler->performBinaryOperation(1e10, 1e10, CalcHandler::Operation::Add);
    
    QVERIFY(result.success);
    QCOMPARE(result.value, 2e10);
}

void TestCalcHandler::testSmallNumbers()
{
    auto result = m_handler->performBinaryOperation(0.0001, 0.0001, CalcHandler::Operation::Add);
    
    QVERIFY(result.success);
    QCOMPARE(result.value, 0.0002);
}

void TestCalcHandler::testChainedOperations()
{
    // 5 + 3 = 8
    m_handler->setOperand(5.0);
    m_handler->setOperation(CalcHandler::Operation::Add);
    auto result = m_handler->performBinaryOperation(5.0, 3.0, CalcHandler::Operation::Add);
    QVERIFY(result.success);
    QCOMPARE(result.value, 8.0);
    
    // 8 * 2 = 16
    m_handler->setOperand(result.value);
    m_handler->setOperation(CalcHandler::Operation::Multiply);
    result = m_handler->performBinaryOperation(result.value, 2.0, CalcHandler::Operation::Multiply);
    QVERIFY(result.success);
    QCOMPARE(result.value, 16.0);
}

QTEST_MAIN(TestCalcHandler)
#include "test_calchandler.moc"
