#include "../src/calculationhistory.h"
#include <QtTest/QtTest>
#include <QTemporaryFile>

class TestCalculationHistory : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    
    void testAddEntry();
    void testAddEntryWithExpressionAndResult();
    void testMaxSize();
    void testClear();
    void testGetAll();
    void testGetLast();
    void testCount();
    void testSaveAndLoad();

private:
    CalculationHistory *m_history;
};

void TestCalculationHistory::init()
{
    m_history = new CalculationHistory();
}

void TestCalculationHistory::cleanup()
{
    delete m_history;
    m_history = nullptr;
}

void TestCalculationHistory::testAddEntry()
{
    m_history->addEntry("5 + 3 = 8");
    
    QCOMPARE(m_history->count(), 1);
    QCOMPARE(m_history->getLast(), QString("5 + 3 = 8"));
}

void TestCalculationHistory::testAddEntryWithExpressionAndResult()
{
    m_history->addEntry("5 + 3", 8.0);
    
    QCOMPARE(m_history->count(), 1);
    QVERIFY(m_history->getLast().contains("5 + 3"));
    QVERIFY(m_history->getLast().contains("8"));
}

void TestCalculationHistory::testMaxSize()
{
    m_history->setMaxSize(3);
    
    m_history->addEntry("1 + 1 = 2");
    m_history->addEntry("2 + 2 = 4");
    m_history->addEntry("3 + 3 = 6");
    m_history->addEntry("4 + 4 = 8");
    
    QCOMPARE(m_history->count(), 3);
    QCOMPARE(m_history->getLast(), QString("4 + 4 = 8"));
}

void TestCalculationHistory::testClear()
{
    m_history->addEntry("5 + 3 = 8");
    m_history->addEntry("10 - 2 = 8");
    
    QCOMPARE(m_history->count(), 2);
    
    m_history->clear();
    
    QCOMPARE(m_history->count(), 0);
    QVERIFY(m_history->getLast().isEmpty());
}

void TestCalculationHistory::testGetAll()
{
    m_history->addEntry("5 + 3 = 8");
    m_history->addEntry("10 - 2 = 8");
    m_history->addEntry("4 * 2 = 8");
    
    QStringList all = m_history->getAll();
    
    QCOMPARE(all.size(), 3);
    QCOMPARE(all.at(0), QString("4 * 2 = 8"));
    QCOMPARE(all.at(1), QString("10 - 2 = 8"));
    QCOMPARE(all.at(2), QString("5 + 3 = 8"));
}

void TestCalculationHistory::testGetLast()
{
    QVERIFY(m_history->getLast().isEmpty());
    
    m_history->addEntry("5 + 3 = 8");
    QCOMPARE(m_history->getLast(), QString("5 + 3 = 8"));
    
    m_history->addEntry("10 - 2 = 8");
    QCOMPARE(m_history->getLast(), QString("10 - 2 = 8"));
}

void TestCalculationHistory::testCount()
{
    QCOMPARE(m_history->count(), 0);
    
    m_history->addEntry("5 + 3 = 8");
    QCOMPARE(m_history->count(), 1);
    
    m_history->addEntry("10 - 2 = 8");
    QCOMPARE(m_history->count(), 2);
    
    m_history->clear();
    QCOMPARE(m_history->count(), 0);
}

void TestCalculationHistory::testSaveAndLoad()
{
    QTemporaryFile tempFile;
    QVERIFY(tempFile.open());
    QString filename = tempFile.fileName();
    tempFile.close();
    
    m_history->addEntry("5 + 3 = 8");
    m_history->addEntry("10 - 2 = 8");
    m_history->addEntry("4 * 2 = 8");
    
    m_history->saveToFile(filename);
    
    CalculationHistory loadedHistory;
    loadedHistory.loadFromFile(filename);
    
    QCOMPARE(loadedHistory.count(), 3);
    QCOMPARE(loadedHistory.getLast(), QString("5 + 3 = 8"));
}

QTEST_MAIN(TestCalculationHistory)
#include "test_calculationhistory.moc"
