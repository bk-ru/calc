#include "uianimations.h"
#include <QGraphicsColorizeEffect>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>

void UIAnimations::flashError(QWidget* widget, int duration)
{
    if (!widget) return;
    
    QString originalStyle = widget->styleSheet();
    
    QPropertyAnimation* animation = new QPropertyAnimation(widget, "styleSheet");
    animation->setDuration(duration);
    animation->setStartValue("background-color: #ffcccc;");
    animation->setEndValue(originalStyle);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    
    QObject::connect(animation, &QPropertyAnimation::finished, animation, &QObject::deleteLater);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void UIAnimations::flashSuccess(QWidget* widget, int duration)
{
    if (!widget) return;
    
    QString originalStyle = widget->styleSheet();
    
    QPropertyAnimation* animation = new QPropertyAnimation(widget, "styleSheet");
    animation->setDuration(duration);
    animation->setStartValue("background-color: #ccffcc;");
    animation->setEndValue(originalStyle);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    
    QObject::connect(animation, &QPropertyAnimation::finished, animation, &QObject::deleteLater);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void UIAnimations::buttonPress(QWidget* button, int duration)
{
    if (!button) return;
    
    QPropertyAnimation* scaleDown = new QPropertyAnimation(button, "geometry");
    scaleDown->setDuration(duration / 2);
    QRect originalGeometry = button->geometry();
    QRect scaledGeometry = originalGeometry.adjusted(2, 2, -2, -2);
    scaleDown->setStartValue(originalGeometry);
    scaleDown->setEndValue(scaledGeometry);
    
    QPropertyAnimation* scaleUp = new QPropertyAnimation(button, "geometry");
    scaleUp->setDuration(duration / 2);
    scaleUp->setStartValue(scaledGeometry);
    scaleUp->setEndValue(originalGeometry);
    
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup();
    group->addAnimation(scaleDown);
    group->addAnimation(scaleUp);
    
    QObject::connect(group, &QSequentialAnimationGroup::finished, group, &QObject::deleteLater);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void UIAnimations::fadeIn(QWidget* widget, int duration)
{
    if (!widget) return;
    
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(effect);
    
    QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(duration);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    
    QObject::connect(animation, &QPropertyAnimation::finished, [effect]() {
        effect->deleteLater();
    });
    QObject::connect(animation, &QPropertyAnimation::finished, animation, &QObject::deleteLater);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void UIAnimations::fadeOut(QWidget* widget, int duration)
{
    if (!widget) return;
    
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(effect);
    
    QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(duration);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    
    QObject::connect(animation, &QPropertyAnimation::finished, [effect]() {
        effect->deleteLater();
    });
    QObject::connect(animation, &QPropertyAnimation::finished, animation, &QObject::deleteLater);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void UIAnimations::shake(QWidget* widget, int duration)
{
    if (!widget) return;
    
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup();
    QPoint originalPos = widget->pos();
    
    int shakeDistance = 5;
    int steps = 4;
    int stepDuration = duration / (steps * 2);
    
    for (int i = 0; i < steps; ++i) {
        QPropertyAnimation* moveRight = new QPropertyAnimation(widget, "pos");
        moveRight->setDuration(stepDuration);
        moveRight->setStartValue(originalPos);
        moveRight->setEndValue(originalPos + QPoint(shakeDistance, 0));
        
        QPropertyAnimation* moveLeft = new QPropertyAnimation(widget, "pos");
        moveLeft->setDuration(stepDuration);
        moveLeft->setStartValue(originalPos + QPoint(shakeDistance, 0));
        moveLeft->setEndValue(originalPos - QPoint(shakeDistance, 0));
        
        group->addAnimation(moveRight);
        group->addAnimation(moveLeft);
    }
    
    QPropertyAnimation* moveBack = new QPropertyAnimation(widget, "pos");
    moveBack->setDuration(stepDuration);
    moveBack->setStartValue(originalPos - QPoint(shakeDistance, 0));
    moveBack->setEndValue(originalPos);
    group->addAnimation(moveBack);
    
    QObject::connect(group, &QSequentialAnimationGroup::finished, group, &QObject::deleteLater);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}
