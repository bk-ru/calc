#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include <QKeyEvent>
#include <QClipboard>
#include <QHBoxLayout>
#include "calchandler.h"
#include "calculationhistory.h"
#include "memorymanager.h"
#include "thememanager.h"
#include "historypanel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Главное окно калькулятора
// Отвечает только за UI-логику: обработку событий кнопок и клавиатуры,
// обновление дисплея. Вся бизнес-логика в CalcHandler.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    // Обработка ввода цифр
    void onNumberButtonClicked(QAbstractButton* button);
    void onDecimalPointClicked();

private slots:
    // Обработка операций
    void onOperatorButtonClicked(QAbstractButton* button);
    void onEqualClicked();
    void onPercentClicked();
    void onSignToggleClicked();
    void onSquareClicked();
    void onSqrtClicked();
    void onReciprocalClicked();

private slots:
    // Управление вводом
    void onDeleteClicked();
    void onClearClicked();
    void onClearEntryClicked();
    void onCopyClicked();
    void onHistoryClicked();

private slots:
    // Память калькулятора
    void onMemoryAddClicked();
    void onMemorySubtractClicked();
    void onMemoryRecallClicked();
    void onMemoryClearClicked();
    void onMemoryStoreClicked();
    void onMemoryDropdownClicked();
    void onMemoryChanged(bool hasValue);
    void onMemoryValueSelected(double value);

private slots:
    // Управление темой
    void onToggleThemeClicked();
    void onThemeChanged(ThemeManager::Theme theme);

private:
    // Инициализация
    void setupUi();
    void connectSignals();

private:
    // Работа с дисплеем
    QString getDisplayText() const;
    void setDisplayText(const QString& text);
    void clearDisplay();
    void showError(const QString& errorMessage);

private:
    // Делегирование в бизнес-логику
    void performCalculation();
    void handleDigitInput(const QString& digit);
    void handleOperatorInput(QChar operatorChar);
    void applyUnaryOperation(CalcHandler::Operation op);

private:
    bool m_operatorClicked;  // Был ли нажат оператор
    bool m_resultDisplayed;  // Отображен ли результат
    QString m_lastExpression; // Последнее выражение для истории

private:
    Ui::MainWindow *ui;
    CalcHandler *m_calcHandler;
    CalculationHistory *m_history;
    MemoryManager *m_memory;
    ThemeManager *m_themeManager;
    HistoryPanel *m_historyPanel;
};

#endif // MAINWINDOW_H
