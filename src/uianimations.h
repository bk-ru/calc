#ifndef UIANIMATIONS_H
#define UIANIMATIONS_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

// Класс для UI анимаций и визуальных эффектов
class UIAnimations
{
public:
    // Анимация ошибки (красная вспышка)
    static void flashError(QWidget* widget, int duration = 500);    
    // Анимация успеха (зеленая вспышка)
    static void flashSuccess(QWidget* widget, int duration = 300);    
    // Анимация нажатия кнопки
    static void buttonPress(QWidget* button, int duration = 100);    
    // Плавное появление
    static void fadeIn(QWidget* widget, int duration = 300);    
    // Плавное исчезновение
    static void fadeOut(QWidget* widget, int duration = 300);    
    // Встряхивание виджета
    static void shake(QWidget* widget, int duration = 500);
private:
    UIAnimations() = default;
};

#endif // UIANIMATIONS_H
