#include "mainwindow.h"

#include <QtTest/QtTest>
#include <QLabel>
#include <QPushButton>

class TestMainWindow : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void testDigitEntry();
    void testDecimalPoint();
    void testAddition();
    void testPercent();
    void testSignToggle();
    void testSquare();
    void testSquareRoot();
    void testReciprocal();
    void testClearEntry();
    void testDelete();
    void testClear();

private:
    QPushButton *button(const char *name) const;
    void click(const char *name) const;
    QString displayText() const;

    MainWindow *m_window = nullptr;
    QLabel *m_display = nullptr;
};

void TestMainWindow::init()
{
    m_window = new MainWindow();
    m_window->show();
    const bool exposed = QTest::qWaitForWindowExposed(m_window);
    QVERIFY(exposed);

    m_display = m_window->findChild<QLabel*>("displayRes");
    QVERIFY(m_display != nullptr);
}

void TestMainWindow::cleanup()
{
    delete m_window;
    m_window = nullptr;
    m_display = nullptr;
}

QPushButton *TestMainWindow::button(const char *name) const
{
    auto *btn = m_window->findChild<QPushButton*>(name);
    if (!btn) {
        QTest::qFail("Button not found", __FILE__, __LINE__);
        return nullptr;
    }
    return btn;
}

void TestMainWindow::click(const char *name) const
{
    QTest::mouseClick(button(name), Qt::LeftButton);
}

QString TestMainWindow::displayText() const
{
    return m_display->text();
}

void TestMainWindow::testDigitEntry()
{
    click("num1");
    click("num2");
    click("num3");

    QCOMPARE(displayText(), QString("123"));
}

void TestMainWindow::testDecimalPoint()
{
    click("comma");
    click("num5");

    QCOMPARE(displayText(), QString("0.5"));
}

void TestMainWindow::testAddition()
{
    click("num2");
    click("operPlus");
    click("num3");
    click("operEqual");

    QCOMPARE(displayText(), QString("5"));
}

void TestMainWindow::testPercent()
{
    click("num5");
    click("num0");
    click("operPercent");

    QCOMPARE(displayText(), QString("0.5"));
}

void TestMainWindow::testSignToggle()
{
    click("num1");
    click("num2");
    click("operSign");

    QCOMPARE(displayText(), QString("-12"));
}

void TestMainWindow::testSquare()
{
    click("num4");
    click("operSquare");

    QCOMPARE(displayText(), QString("16"));
}

void TestMainWindow::testSquareRoot()
{
    click("num9");
    click("operSqrt");

    QCOMPARE(displayText(), QString("3"));
}

void TestMainWindow::testReciprocal()
{
    click("num4");
    click("operNone");

    QCOMPARE(displayText(), QString("0.25"));
}

void TestMainWindow::testClearEntry()
{
    click("num1");
    click("num2");
    click("buttonCE");

    QVERIFY(displayText().isEmpty());
}

void TestMainWindow::testDelete()
{
    click("num1");
    click("num2");
    click("num3");
    click("buttonDel");

    QCOMPARE(displayText(), QString("12"));
}

void TestMainWindow::testClear()
{
    click("num7");
    click("buttonC");

    QVERIFY(displayText().isEmpty());
}

QTEST_MAIN(TestMainWindow)
#include "test_mainwindow.moc"
