#ifndef HISTORYPANEL_H
#define HISTORYPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QPropertyAnimation>
#include "calculationhistory.h"

// Виджет для отображения одной записи истории
class HistoryItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryItemWidget(const QString& expression, const QString& result, QWidget* parent = nullptr);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    QLabel* m_expressionLabel;
    QLabel* m_resultLabel;
};

// Боковая панель истории
class HistoryPanel : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryPanel(CalculationHistory* history, QWidget* parent = nullptr);
    
public:
    void updateHistory();
    void show();
    void hide();
    bool isVisible() const;

signals:
    void closed();

private slots:
    void onClearClicked();

private:
    void setupUi();
    void addHistoryItem(const QString& fullExpression);
    void parseExpression(const QString& fullExpression, QString& expression, QString& result);

private:
    CalculationHistory* m_history;
    QVBoxLayout* m_mainLayout;
    QScrollArea* m_scrollArea;
    QWidget* m_scrollContent;
    QVBoxLayout* m_scrollLayout;
    QPushButton* m_clearButton;
    QLabel* m_titleLabel;
    QLabel* m_emptyLabel;
    bool m_isVisible;
};

#endif // HISTORYPANEL_H
