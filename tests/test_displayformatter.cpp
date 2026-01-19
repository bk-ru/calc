#include "displayformatter.h"
#include "calculatorconfig.h"
#include <QtTest/QtTest>

/**
 * @brief Тесты для класса DisplayFormatter
 */
class TestDisplayFormatter : public QObject
{
    Q_OBJECT

private slots:
    void testFormatNumber();
    void testIsValidNumber();
    void testToDouble();
    void testRemoveTrailingDecimal();
    void testHasDecimalPoint();
};

void TestDisplayFormatter::testFormatNumber()
{
    QString result = DisplayFormatter::formatNumber(42.0);
    QVERIFY(!result.isEmpty());
    
    result = DisplayFormatter::formatNumber(3.14159, 3);
    QVERIFY(result.contains("3.14"));
}

void TestDisplayFormatter::testIsValidNumber()
{
    QVERIFY(DisplayFormatter::isValidNumber("42"));
    QVERIFY(DisplayFormatter::isValidNumber("3.14"));
    QVERIFY(DisplayFormatter::isValidNumber("-5.5"));
    QVERIFY(!DisplayFormatter::isValidNumber(""));
    QVERIFY(!DisplayFormatter::isValidNumber("abc"));
}

void TestDisplayFormatter::testToDouble()
{
    bool ok = false;
    double value = DisplayFormatter::toDouble("42.5", &ok);
    QVERIFY(ok);
    QCOMPARE(value, 42.5);
    
    value = DisplayFormatter::toDouble("invalid", &ok);
    QVERIFY(!ok);
}

void TestDisplayFormatter::testRemoveTrailingDecimal()
{
    QCOMPARE(DisplayFormatter::removeTrailingDecimal("42."), "42");
    QCOMPARE(DisplayFormatter::removeTrailingDecimal("42.5"), "42.5");
    QCOMPARE(DisplayFormatter::removeTrailingDecimal("42"), "42");
}

void TestDisplayFormatter::testHasDecimalPoint()
{
    QVERIFY(DisplayFormatter::hasDecimalPoint("3.14"));
    QVERIFY(DisplayFormatter::hasDecimalPoint("42."));
    QVERIFY(!DisplayFormatter::hasDecimalPoint("42"));
    QVERIFY(!DisplayFormatter::hasDecimalPoint(""));
}

QTEST_MAIN(TestDisplayFormatter)
#include "test_displayformatter.moc"
