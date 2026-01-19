#include "inputvalidator.h"
#include <QtTest/QtTest>

/**
 * @brief Тесты для класса InputValidator
 */
class TestInputValidator : public QObject
{
    Q_OBJECT

private slots:
    void testCanAddDigit();
    void testCanAddDecimalPoint();
    void testIsNotEmpty();
    void testIsLengthExceeded();
};

void TestInputValidator::testCanAddDigit()
{
    QVERIFY(InputValidator::canAddDigit("123", 10));
    QVERIFY(!InputValidator::canAddDigit("1234567890", 10));
    QVERIFY(InputValidator::canAddDigit("", 10));
}

void TestInputValidator::testCanAddDecimalPoint()
{
    QVERIFY(InputValidator::canAddDecimalPoint("123", 10));
    QVERIFY(!InputValidator::canAddDecimalPoint("12.3", 10));
    QVERIFY(!InputValidator::canAddDecimalPoint("123456789", 10));
    QVERIFY(InputValidator::canAddDecimalPoint("", 10));
}

void TestInputValidator::testIsNotEmpty()
{
    QVERIFY(InputValidator::isNotEmpty("123"));
    QVERIFY(!InputValidator::isNotEmpty(""));
}

void TestInputValidator::testIsLengthExceeded()
{
    QVERIFY(InputValidator::isLengthExceeded("1234567890", 10));
    QVERIFY(!InputValidator::isLengthExceeded("123", 10));
}

QTEST_MAIN(TestInputValidator)
#include "test_inputvalidator.moc"
