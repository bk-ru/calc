#include "../src/thememanager.h"
#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QSettings>

class TestThemeManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    void testInitialTheme();
    void testSetTheme();
    void testGetStyleSheet();
    void testThemeChanged();
    void testSaveAndLoad();
    void testThemeName();
    void testSystemTheme();

private:
    ThemeManager *m_themeManager;
};

void TestThemeManager::initTestCase()
{
    // Регистрируем тип для QSignalSpy
    qRegisterMetaType<ThemeManager::Theme>("ThemeManager::Theme");
    
    // Очистить настройки перед тестами
    QSettings settings("Calculator", "Theme");
    settings.clear();
}

void TestThemeManager::cleanupTestCase()
{
    // Очистить настройки после тестов
    QSettings settings("Calculator", "Theme");
    settings.clear();
}

void TestThemeManager::init()
{
    m_themeManager = new ThemeManager();
}

void TestThemeManager::cleanup()
{
    delete m_themeManager;
    m_themeManager = nullptr;
}

void TestThemeManager::testInitialTheme()
{
    QCOMPARE(m_themeManager->currentTheme(), ThemeManager::Theme::Light);
}

void TestThemeManager::testSetTheme()
{
    m_themeManager->setTheme(ThemeManager::Theme::Dark);
    QCOMPARE(m_themeManager->currentTheme(), ThemeManager::Theme::Dark);
    
    m_themeManager->setTheme(ThemeManager::Theme::Light);
    QCOMPARE(m_themeManager->currentTheme(), ThemeManager::Theme::Light);
}

void TestThemeManager::testGetStyleSheet()
{
    QString lightStyle = m_themeManager->getStyleSheet(ThemeManager::Theme::Light);
    QVERIFY(!lightStyle.isEmpty());
    QVERIFY(lightStyle.contains("background-color"));
    
    QString darkStyle = m_themeManager->getStyleSheet(ThemeManager::Theme::Dark);
    QVERIFY(!darkStyle.isEmpty());
    QVERIFY(darkStyle.contains("background-color"));
    
    // Темы должны отличаться
    QVERIFY(lightStyle != darkStyle);
}

void TestThemeManager::testThemeChanged()
{
    // Убедимся, что тип зарегистрирован
    qRegisterMetaType<ThemeManager::Theme>("ThemeManager::Theme");
    
    QSignalSpy spy(m_themeManager, &ThemeManager::themeChanged);
    QVERIFY(spy.isValid());
    
    m_themeManager->setTheme(ThemeManager::Theme::Dark);
    
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).value<ThemeManager::Theme>(), ThemeManager::Theme::Dark);
    
    // Установка той же темы не должна вызывать сигнал
    m_themeManager->setTheme(ThemeManager::Theme::Dark);
    QCOMPARE(spy.count(), 0);
}

void TestThemeManager::testSaveAndLoad()
{
    m_themeManager->setTheme(ThemeManager::Theme::Dark);
    m_themeManager->saveThemePreference();
    
    ThemeManager newManager;
    newManager.loadThemePreference();
    
    QCOMPARE(newManager.currentTheme(), ThemeManager::Theme::Dark);
}

void TestThemeManager::testThemeName()
{
    QCOMPARE(ThemeManager::themeName(ThemeManager::Theme::Light), QString("Светлая"));
    QCOMPARE(ThemeManager::themeName(ThemeManager::Theme::Dark), QString("Темная"));
    QCOMPARE(ThemeManager::themeName(ThemeManager::Theme::System), QString("Системная"));
}

void TestThemeManager::testSystemTheme()
{
    // Установка системной темы должна выбрать Light или Dark
    m_themeManager->setTheme(ThemeManager::Theme::System);
    
    ThemeManager::Theme current = m_themeManager->currentTheme();
    QVERIFY(current == ThemeManager::Theme::Light || current == ThemeManager::Theme::Dark);
}

QTEST_MAIN(TestThemeManager)
#include "test_thememanager.moc"
