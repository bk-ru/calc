#include "thememanager.h"
#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QPalette>

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
    , m_currentTheme(Theme::Light)
{
}

void ThemeManager::setTheme(Theme theme)
{
    if (theme == Theme::System) {
        theme = detectSystemTheme();
    }
    
    if (m_currentTheme != theme) {
        m_currentTheme = theme;
        applyTheme(theme);
        emit themeChanged(theme);
        qDebug() << "Тема изменена на:" << themeName(theme);
    }
}

ThemeManager::Theme ThemeManager::currentTheme() const
{
    return m_currentTheme;
}

QString ThemeManager::getStyleSheet(Theme theme) const
{
    switch (theme) {
        case Theme::Dark:
            return getDarkThemeStyleSheet();
        case Theme::Light:
        case Theme::System:
        default:
            return getLightThemeStyleSheet();
    }
}

void ThemeManager::applyTheme(Theme theme)
{
    QString styleSheet = getStyleSheet(theme);
    qApp->setStyleSheet(styleSheet);
}

void ThemeManager::saveThemePreference()
{
    QSettings settings("Calculator", "Theme");
    settings.setValue("theme", static_cast<int>(m_currentTheme));
    qDebug() << "Настройки темы сохранены";
}

void ThemeManager::loadThemePreference()
{
    QSettings settings("Calculator", "Theme");
    int themeValue = settings.value("theme", static_cast<int>(Theme::Light)).toInt();
    Theme theme = static_cast<Theme>(themeValue);
    
    setTheme(theme);
    qDebug() << "Настройки темы загружены:" << themeName(theme);
}

QString ThemeManager::themeName(Theme theme)
{
    switch (theme) {
        case Theme::Light:
            return "Светлая";
        case Theme::Dark:
            return "Темная";
        case Theme::System:
            return "Системная";
        default:
            return "Неизвестная";
    }
}

QString ThemeManager::getLightThemeStyleSheet() const
{
    return R"(
        QMainWindow {
            background-color: #f5f5f5;
        }
        
        QLabel#displayRes {
            background-color: #ffffff;
            border: 2px solid #e0e0e0;
            border-radius: 5px;
            padding: 10px;
            color: #000000;
            font-size: 24px;
            font-weight: bold;
        }
        
        QPushButton {
            background-color: #ffffff;
            border: 1px solid #d0d0d0;
            border-radius: 5px;
            padding: 10px;
            font-size: 16px;
            color: #000000;
        }
        
        QPushButton:hover {
            background-color: #f0f0f0;
            border: 1px solid #b0b0b0;
        }
        
        QPushButton:pressed {
            background-color: #e0e0e0;
        }
        
        QPushButton[text="="] {
            background-color: #4CAF50;
            color: white;
            font-weight: bold;
        }
        
        QPushButton[text="="]:hover {
            background-color: #45a049;
        }
        
        QPushButton[text="C"], QPushButton[text="CE"], QPushButton[text="DEL"] {
            background-color: #f44336;
            color: white;
        }
        
        QPushButton[text="C"]:hover, QPushButton[text="CE"]:hover, QPushButton[text="DEL"]:hover {
            background-color: #da190b;
        }
        
        QDialog {
            background-color: #f5f5f5;
        }
        
        QListWidget {
            background-color: #ffffff;
            border: 1px solid #d0d0d0;
            border-radius: 5px;
            color: #000000;
        }
        
        QListWidget::item:alternate {
            background-color: #f9f9f9;
        }
        
        QListWidget::item:selected {
            background-color: #2196F3;
            color: white;
        }
    )";
}

QString ThemeManager::getDarkThemeStyleSheet() const
{
    return R"(
        QMainWindow {
            background-color: #1e1e1e;
        }
        
        QLabel#displayRes {
            background-color: #2d2d2d;
            border: 2px solid #3d3d3d;
            border-radius: 5px;
            padding: 10px;
            color: #ffffff;
            font-size: 24px;
            font-weight: bold;
        }
        
        QPushButton {
            background-color: #2d2d2d;
            border: 1px solid #3d3d3d;
            border-radius: 5px;
            padding: 10px;
            font-size: 16px;
            color: #ffffff;
        }
        
        QPushButton:hover {
            background-color: #3d3d3d;
            border: 1px solid #4d4d4d;
        }
        
        QPushButton:pressed {
            background-color: #4d4d4d;
        }
        
        QPushButton[text="="] {
            background-color: #4CAF50;
            color: white;
            font-weight: bold;
        }
        
        QPushButton[text="="]:hover {
            background-color: #45a049;
        }
        
        QPushButton[text="C"], QPushButton[text="CE"], QPushButton[text="DEL"] {
            background-color: #f44336;
            color: white;
        }
        
        QPushButton[text="C"]:hover, QPushButton[text="CE"]:hover, QPushButton[text="DEL"]:hover {
            background-color: #da190b;
        }
        
        QDialog {
            background-color: #1e1e1e;
            color: #ffffff;
        }
        
        QLabel {
            color: #ffffff;
        }
        
        QListWidget {
            background-color: #2d2d2d;
            border: 1px solid #3d3d3d;
            border-radius: 5px;
            color: #ffffff;
        }
        
        QListWidget::item:alternate {
            background-color: #252525;
        }
        
        QListWidget::item:selected {
            background-color: #2196F3;
            color: white;
        }
        
        QPushButton {
            background-color: #2d2d2d;
            color: #ffffff;
        }
        
        QPushButton:hover {
            background-color: #3d3d3d;
        }
    )";
}

ThemeManager::Theme ThemeManager::detectSystemTheme() const
{
    // Определение системной темы через палитру
    QPalette palette = qApp->palette();
    QColor backgroundColor = palette.color(QPalette::Window);
    
    // Если фон темный, используем темную тему
    int brightness = (backgroundColor.red() + backgroundColor.green() + backgroundColor.blue()) / 3;
    
    return (brightness < 128) ? Theme::Dark : Theme::Light;
}
