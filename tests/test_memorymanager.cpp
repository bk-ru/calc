#include "../src/memorymanager.h"
#include <QtTest/QtTest>
#include <QSignalSpy>

class TestMemoryManager : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    
    void testInitialState();
    void testAdd();
    void testSubtract();
    void testRecall();
    void testClear();
    void testHasValue();
    void testMultipleOperations();
    void testSignalEmission();
    void testNegativeValues();
    void testZeroValue();
    
    // Новые тесты для MS и M˅
    void testStore();
    void testAddToList();
    void testGetMemoryList();
    void testRecallFromList();
    void testRemoveFromList();
    void testClearList();
    void testListMaxSize();
    void testListSignalEmission();

private:
    MemoryManager *m_memory;
};

void TestMemoryManager::init()
{
    m_memory = new MemoryManager();
}

void TestMemoryManager::cleanup()
{
    delete m_memory;
    m_memory = nullptr;
}

void TestMemoryManager::testInitialState()
{
    QCOMPARE(m_memory->value(), 0.0);
    QVERIFY(!m_memory->hasValue());
    QCOMPARE(m_memory->recall(), 0.0);
}

void TestMemoryManager::testAdd()
{
    m_memory->add(5.0);
    QCOMPARE(m_memory->value(), 5.0);
    QVERIFY(m_memory->hasValue());
    
    m_memory->add(3.0);
    QCOMPARE(m_memory->value(), 8.0);
    QVERIFY(m_memory->hasValue());
}

void TestMemoryManager::testSubtract()
{
    m_memory->add(10.0);
    m_memory->subtract(3.0);
    
    QCOMPARE(m_memory->value(), 7.0);
    QVERIFY(m_memory->hasValue());
}

void TestMemoryManager::testRecall()
{
    m_memory->add(42.0);
    
    double recalled = m_memory->recall();
    QCOMPARE(recalled, 42.0);
    
    // Recall не должен изменять значение
    QCOMPARE(m_memory->value(), 42.0);
}

void TestMemoryManager::testClear()
{
    m_memory->add(100.0);
    QVERIFY(m_memory->hasValue());
    
    m_memory->clear();
    
    QCOMPARE(m_memory->value(), 0.0);
    QVERIFY(!m_memory->hasValue());
}

void TestMemoryManager::testHasValue()
{
    QVERIFY(!m_memory->hasValue());
    
    m_memory->add(0.1);
    QVERIFY(m_memory->hasValue());
    
    m_memory->clear();
    QVERIFY(!m_memory->hasValue());
    
    m_memory->add(-5.0);
    QVERIFY(m_memory->hasValue());
}

void TestMemoryManager::testMultipleOperations()
{
    m_memory->add(10.0);      // 10
    m_memory->add(5.0);       // 15
    m_memory->subtract(3.0);  // 12
    m_memory->add(8.0);       // 20
    m_memory->subtract(5.0);  // 15
    
    QCOMPARE(m_memory->value(), 15.0);
}

void TestMemoryManager::testSignalEmission()
{
    QSignalSpy spy(m_memory, &MemoryManager::memoryChanged);
    
    // Добавление значения должно вызвать сигнал
    m_memory->add(5.0);
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toBool() == true);
    
    // Еще одно добавление
    m_memory->add(3.0);
    QCOMPARE(spy.count(), 1);
    spy.takeFirst();
    
    // Очистка должна вызвать сигнал
    m_memory->clear();
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toBool() == false);
}

void TestMemoryManager::testNegativeValues()
{
    m_memory->add(-10.0);
    QCOMPARE(m_memory->value(), -10.0);
    QVERIFY(m_memory->hasValue());
    
    m_memory->subtract(-5.0);  // -10 - (-5) = -5
    QCOMPARE(m_memory->value(), -5.0);
}

void TestMemoryManager::testZeroValue()
{
    m_memory->add(5.0);
    m_memory->subtract(5.0);
    
    // Значение 0, но технически это результат операций
    QCOMPARE(m_memory->value(), 0.0);
    QVERIFY(!m_memory->hasValue());  // qFuzzyIsNull должен вернуть true
}

void TestMemoryManager::testStore()
{
    m_memory->add(10.0);
    QCOMPARE(m_memory->value(), 10.0);
    
    // MS должен заменить значение
    m_memory->store(42.0);
    QCOMPARE(m_memory->value(), 42.0);
    QVERIFY(m_memory->hasValue());
    
    // Еще одно сохранение
    m_memory->store(100.0);
    QCOMPARE(m_memory->value(), 100.0);
}

void TestMemoryManager::testAddToList()
{
    QCOMPARE(m_memory->listSize(), 0);
    
    m_memory->addToList(10.0);
    QCOMPARE(m_memory->listSize(), 1);
    
    m_memory->addToList(20.0);
    QCOMPARE(m_memory->listSize(), 2);
    
    m_memory->addToList(30.0);
    QCOMPARE(m_memory->listSize(), 3);
}

void TestMemoryManager::testGetMemoryList()
{
    m_memory->addToList(10.0);
    m_memory->addToList(20.0);
    m_memory->addToList(30.0);
    
    QList<double> list = m_memory->getMemoryList();
    
    QCOMPARE(list.size(), 3);
    // Последний добавленный должен быть первым (prepend)
    QCOMPARE(list.at(0), 30.0);
    QCOMPARE(list.at(1), 20.0);
    QCOMPARE(list.at(2), 10.0);
}

void TestMemoryManager::testRecallFromList()
{
    m_memory->addToList(10.0);
    m_memory->addToList(20.0);
    m_memory->addToList(30.0);
    
    // Вспомнить второй элемент
    m_memory->recallFromList(1);
    QCOMPARE(m_memory->value(), 20.0);
    
    // Вспомнить первый элемент
    m_memory->recallFromList(0);
    QCOMPARE(m_memory->value(), 30.0);
}

void TestMemoryManager::testRemoveFromList()
{
    m_memory->addToList(10.0);
    m_memory->addToList(20.0);
    m_memory->addToList(30.0);
    
    QCOMPARE(m_memory->listSize(), 3);
    
    // Удалить средний элемент
    m_memory->removeFromList(1);
    QCOMPARE(m_memory->listSize(), 2);
    
    QList<double> list = m_memory->getMemoryList();
    QCOMPARE(list.at(0), 30.0);
    QCOMPARE(list.at(1), 10.0);
}

void TestMemoryManager::testClearList()
{
    m_memory->addToList(10.0);
    m_memory->addToList(20.0);
    m_memory->addToList(30.0);
    
    QCOMPARE(m_memory->listSize(), 3);
    
    m_memory->clearList();
    
    QCOMPARE(m_memory->listSize(), 0);
    QVERIFY(m_memory->getMemoryList().isEmpty());
}

void TestMemoryManager::testListMaxSize()
{
    // Добавить больше максимума (10 элементов)
    for (int i = 0; i < 15; ++i) {
        m_memory->addToList(i * 10.0);
    }
    
    // Должно быть не больше 10
    QCOMPARE(m_memory->listSize(), 10);
    
    QList<double> list = m_memory->getMemoryList();
    // Последний добавленный (140) должен быть первым
    QCOMPARE(list.at(0), 140.0);
    // Первый добавленный (0) должен быть удален
    QVERIFY(!list.contains(0.0));
    QVERIFY(!list.contains(10.0));
    QVERIFY(!list.contains(20.0));
    QVERIFY(!list.contains(30.0));
    QVERIFY(!list.contains(40.0));
}

void TestMemoryManager::testListSignalEmission()
{
    QSignalSpy spy(m_memory, &MemoryManager::memoryListChanged);
    
    // Добавление в список должно вызвать сигнал
    m_memory->addToList(10.0);
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 1);
    
    // Еще одно добавление
    m_memory->addToList(20.0);
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 2);
    
    // Удаление из списка
    m_memory->removeFromList(0);
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 1);
    
    // Очистка списка
    m_memory->clearList();
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 0);
}

QTEST_MAIN(TestMemoryManager)
#include "test_memorymanager.moc"
