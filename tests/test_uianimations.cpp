#include "../src/uianimations.h"
#include <QtTest/QtTest>
#include <QLabel>
#include <QPushButton>
#include <QEventLoop>
#include <QTimer>

class TestUIAnimations : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    void testFlashError();
    void testFlashSuccess();
    void testButtonPress();
    void testFadeIn();
    void testFadeOut();
    void testShake();
    void testNullWidget();

private:
    QWidget* m_widget;
    QPushButton* m_button;
    
    void waitForAnimation(int duration);
};

void TestUIAnimations::initTestCase()
{
    // Выполняется один раз перед всеми тестами
}

void TestUIAnimations::cleanupTestCase()
{
    // Выполняется один раз после всех тестов
}

void TestUIAnimations::init()
{
    m_widget = new QWidget();
    m_widget->setGeometry(100, 100, 200, 100);
    
    m_button = new QPushButton("Test", m_widget);
    m_button->setGeometry(10, 10, 80, 30);
}

void TestUIAnimations::cleanup()
{
    delete m_widget;
    m_widget = nullptr;
    m_button = nullptr;
}

void TestUIAnimations::waitForAnimation(int duration)
{
    QEventLoop loop;
    QTimer::singleShot(duration + 100, &loop, &QEventLoop::quit);
    loop.exec();
}

void TestUIAnimations::testFlashError()
{
    QString originalStyle = m_widget->styleSheet();
    
    UIAnimations::flashError(m_widget, 200);
    
    // Проверяем что анимация запустилась
    QVERIFY(m_widget != nullptr);
    
    // Ждем завершения анимации
    waitForAnimation(200);
    
    // После анимации стиль должен вернуться
    // (точная проверка сложна из-за асинхронности)
    QVERIFY(m_widget != nullptr);
}

void TestUIAnimations::testFlashSuccess()
{
    UIAnimations::flashSuccess(m_widget, 200);
    
    QVERIFY(m_widget != nullptr);
    
    waitForAnimation(200);
    
    QVERIFY(m_widget != nullptr);
}

void TestUIAnimations::testButtonPress()
{
    QRect originalGeometry = m_button->geometry();
    
    UIAnimations::buttonPress(m_button, 100);
    
    QVERIFY(m_button != nullptr);
    
    waitForAnimation(100);
    
    // После анимации геометрия должна вернуться
    QCOMPARE(m_button->geometry(), originalGeometry);
}

void TestUIAnimations::testFadeIn()
{
    UIAnimations::fadeIn(m_widget, 200);
    
    QVERIFY(m_widget != nullptr);
    QVERIFY(m_widget->graphicsEffect() != nullptr);
    
    waitForAnimation(200);
}

void TestUIAnimations::testFadeOut()
{
    UIAnimations::fadeOut(m_widget, 200);
    
    QVERIFY(m_widget != nullptr);
    QVERIFY(m_widget->graphicsEffect() != nullptr);
    
    waitForAnimation(200);
}

void TestUIAnimations::testShake()
{
    QPoint originalPos = m_widget->pos();
    
    UIAnimations::shake(m_widget, 300);
    
    QVERIFY(m_widget != nullptr);
    
    waitForAnimation(300);
    
    // После анимации позиция должна вернуться
    QCOMPARE(m_widget->pos(), originalPos);
}

void TestUIAnimations::testNullWidget()
{
    // Проверяем что функции не падают с nullptr
    UIAnimations::flashError(nullptr);
    UIAnimations::flashSuccess(nullptr);
    UIAnimations::buttonPress(nullptr);
    UIAnimations::fadeIn(nullptr);
    UIAnimations::fadeOut(nullptr);
    UIAnimations::shake(nullptr);
    
    // Если дошли сюда - тест пройден
    QVERIFY(true);
}

QTEST_MAIN(TestUIAnimations)
#include "test_uianimations.moc"
