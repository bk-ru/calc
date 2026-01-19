#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QMetaType>
#include <QString>

// Класс для управления темами оформления
class ThemeManager : public QObject
{
    Q_OBJECT

public:
    enum class Theme {
        Light,
        Dark,
        System  // Следовать системной теме
    };
    Q_ENUM(Theme)

    explicit ThemeManager(QObject *parent = nullptr);
    ~ThemeManager() override = default;

    void setTheme(Theme theme);
    Theme currentTheme() const;
    
    QString getStyleSheet(Theme theme) const;
    void applyTheme(Theme theme);
    
    void saveThemePreference();
    void loadThemePreference();
    
    static QString themeName(Theme theme);

signals:
    void themeChanged(ThemeManager::Theme theme);

private:
    Theme m_currentTheme;
    
    QString getLightThemeStyleSheet() const;
    QString getDarkThemeStyleSheet() const;
    Theme detectSystemTheme() const;
};

Q_DECLARE_METATYPE(ThemeManager::Theme)

#endif // THEMEMANAGER_H
