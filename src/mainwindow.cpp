#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "calchandler.h"
#include "calculatorconfig.h"
#include "displayformatter.h"
#include "inputvalidator.h"
#include "calculationhistory.h"
#include "historydialog.h"
#include "historypanel.h"
#include "memorymanager.h"
#include "memorydropdowndialog.h"
#include "uianimations.h"
#include "thememanager.h"

#include <QDebug>
#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_calcHandler(new CalcHandler(this))
    , m_history(new CalculationHistory(this))
    , m_memory(new MemoryManager(this))
    , m_themeManager(new ThemeManager(this))
    , m_historyPanel(nullptr)
    , m_operatorClicked(false)
    , m_resultDisplayed(false)
{
    setupUi();
    m_history->loadFromFile("calculator_history.txt");
    
    connect(m_memory, &MemoryManager::memoryChanged, 
            this, &MainWindow::onMemoryChanged);
    
    connect(m_themeManager, &ThemeManager::themeChanged,
            this, &MainWindow::onThemeChanged);
    
    m_themeManager->loadThemePreference();
}

MainWindow::~MainWindow()
{
    m_history->saveToFile("calculator_history.txt");
    m_themeManager->saveThemePreference();
    delete ui;
}

void MainWindow::setupUi()
{
    ui->setupUi(this);
    clearDisplay();
    
    // Создать боковую панель истории
    m_historyPanel = new HistoryPanel(m_history, this);
    
    // Создать layout для центрального виджета с панелью истории
    QWidget* container = new QWidget(this);
    QHBoxLayout* containerLayout = new QHBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->setSpacing(0);
    
    // Добавить оригинальный центральный виджет и панель истории
    QWidget* originalCentral = takeCentralWidget();
    if (!originalCentral) {
        originalCentral = ui->centralwidget;
    }
    originalCentral->setParent(container);
    m_historyPanel->setParent(container);
    containerLayout->addWidget(originalCentral);
    containerLayout->addWidget(m_historyPanel);
    
    setCentralWidget(container);
    
    connectSignals();
}

void MainWindow::connectSignals()
{
    connect(ui->groupNums,
            static_cast<void (QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),
            this, &MainWindow::onNumberButtonClicked);
    
    connect(ui->groupStandOper,
            static_cast<void (QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),
            this, &MainWindow::onOperatorButtonClicked);

    connect(ui->comma, &QPushButton::clicked, this, &MainWindow::onDecimalPointClicked);
    connect(ui->operEqual, &QPushButton::clicked, this, &MainWindow::onEqualClicked);
    connect(ui->operPercent, &QPushButton::clicked, this, &MainWindow::onPercentClicked);
    connect(ui->operSign, &QPushButton::clicked, this, &MainWindow::onSignToggleClicked);
    connect(ui->operSquare, &QPushButton::clicked, this, &MainWindow::onSquareClicked);
    connect(ui->operSqrt, &QPushButton::clicked, this, &MainWindow::onSqrtClicked);
    connect(ui->operNone, &QPushButton::clicked, this, &MainWindow::onReciprocalClicked);
    connect(ui->buttonDel, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
    connect(ui->buttonC, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->buttonCE, &QPushButton::clicked, this, &MainWindow::onClearEntryClicked);
    
    // Подключение кнопок памяти
    connect(ui->buttonMemoryAdd, &QPushButton::clicked, this, &MainWindow::onMemoryAddClicked);
    connect(ui->buttonMemorySubtract, &QPushButton::clicked, this, &MainWindow::onMemorySubtractClicked);
    connect(ui->buttonMemoryRecall, &QPushButton::clicked, this, &MainWindow::onMemoryRecallClicked);
    connect(ui->buttonMemoryClear, &QPushButton::clicked, this, &MainWindow::onMemoryClearClicked);
    connect(ui->buttonMemoryStore, &QPushButton::clicked, this, &MainWindow::onMemoryStoreClicked);
    connect(ui->buttonMemoryDropdown, &QPushButton::clicked, this, &MainWindow::onMemoryDropdownClicked);
    
    // Подключение действий меню
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::onCopyClicked);
    connect(ui->actionHistory, &QAction::triggered, this, &MainWindow::onHistoryClicked);
    connect(ui->actionTheme, &QAction::triggered, this, &MainWindow::onToggleThemeClicked);
}

void MainWindow::onNumberButtonClicked(QAbstractButton *button)
{
    handleDigitInput(button->text());
}

void MainWindow::handleDigitInput(const QString& digit)
{
    QString displayText = getDisplayText();
    
    if (m_operatorClicked || m_resultDisplayed) {
        displayText.clear();
        m_operatorClicked = false;
        m_resultDisplayed = false;
    }
    
    if (!InputValidator::canAddDigit(displayText, CalculatorConfig::MAX_DIGIT_LENGTH)) {
        qDebug() << "Достигнут лимит символов";
        UIAnimations::shake(ui->displayRes, 300);
        UIAnimations::flashError(ui->displayRes, 200);
        return;
    }
    
    displayText.append(digit);
    setDisplayText(displayText);
}

void MainWindow::onDecimalPointClicked()
{
    QString displayText = getDisplayText();

    if (m_operatorClicked || m_resultDisplayed) {
        displayText.clear();
        m_operatorClicked = false;
        m_resultDisplayed = false;
    }
    
    if (!InputValidator::canAddDecimalPoint(displayText, CalculatorConfig::MAX_DIGIT_LENGTH)) {
        return;
    }
    
    if (displayText.isEmpty()) {
        displayText = CalculatorConfig::ZERO_WITH_DECIMAL;
    } else {
        displayText.append(CalculatorConfig::DECIMAL_SEPARATOR);
    }
    
    setDisplayText(displayText);
}

void MainWindow::onOperatorButtonClicked(QAbstractButton *button)
{
    handleOperatorInput(button->text().at(0));
}

void MainWindow::handleOperatorInput(QChar operatorChar)
{
    const bool wasOperatorClicked = m_operatorClicked;
    QString displayText = getDisplayText();
    if (InputValidator::isNotEmpty(displayText) && !DisplayFormatter::isValidNumber(displayText)) {
        showError(CalculatorConfig::ERROR_INVALID_INPUT);
        return;
    }
    
    if (m_calcHandler->hasStoredValue()
        && !m_operatorClicked
        && !m_resultDisplayed
        && m_calcHandler->currentOperation() != CalcHandler::Operation::None) {
        if (InputValidator::isNotEmpty(displayText)) {
            performCalculation();
            displayText = getDisplayText();
        }
    }
    
    if (!wasOperatorClicked && InputValidator::isNotEmpty(displayText)) {
        bool ok = false;
        double value = DisplayFormatter::toDouble(displayText, &ok);
        if (!ok) {
            showError(CalculatorConfig::ERROR_INVALID_INPUT);
            return;
        }
        m_calcHandler->setOperand(value);
        m_lastExpression = displayText;
    }
    
    CalcHandler::Operation op = CalcHandler::operationFromChar(operatorChar);
    m_calcHandler->setOperation(op);
    
    if (!m_lastExpression.isEmpty()) {
        if (wasOperatorClicked && m_lastExpression.size() >= 3) {
            m_lastExpression.chop(3);
        }
        m_lastExpression += QString(" %1 ").arg(operatorChar);
    }
    
    m_operatorClicked = true;
    m_resultDisplayed = false;
}

void MainWindow::onEqualClicked()
{
    if (!m_calcHandler->hasStoredValue()) {
        return;
    }

    if (m_calcHandler->currentOperation() == CalcHandler::Operation::None) {
        return;
    }
    
    performCalculation();
}

void MainWindow::performCalculation()
{
    QString displayText = getDisplayText();
    
    if (displayText.isEmpty()) {
        return;
    }
    
    displayText = DisplayFormatter::removeTrailingDecimal(displayText);
    
    bool ok = false;
    double operand = DisplayFormatter::toDouble(displayText, &ok);
    if (!ok) {
        showError(CalculatorConfig::ERROR_INVALID_INPUT);
        return;
    }
    double storedValue = m_calcHandler->storedValue();
    CalcHandler::Operation op = m_calcHandler->currentOperation();
    
    CalcHandler::CalculationResult result = 
        m_calcHandler->performBinaryOperation(storedValue, operand, op);
    
    if (result.success) {
        QString formattedResult = DisplayFormatter::formatNumber(
            result.value, CalculatorConfig::MAX_DIGIT_LENGTH);
        setDisplayText(formattedResult);
        
        QString fullExpression = m_lastExpression + displayText + " = " + formattedResult;
        m_history->addEntry(fullExpression);
        m_lastExpression.clear();
        
        m_resultDisplayed = true;
    } else {
        showError(result.errorMessage);
    }
    
    m_operatorClicked = false;
}

void MainWindow::onPercentClicked()
{
    applyUnaryOperation(CalcHandler::Operation::Percent);
}

void MainWindow::onSignToggleClicked()
{
    applyUnaryOperation(CalcHandler::Operation::Negate);
}

void MainWindow::onSquareClicked()
{
    applyUnaryOperation(CalcHandler::Operation::Square);
}

void MainWindow::onSqrtClicked()
{
    applyUnaryOperation(CalcHandler::Operation::SquareRoot);
}

void MainWindow::onReciprocalClicked()
{
    applyUnaryOperation(CalcHandler::Operation::Reciprocal);
}

void MainWindow::onDeleteClicked()
{
    QString displayText = getDisplayText();
    
    if (displayText.isEmpty()) {
        return;
    }
    
    displayText.chop(1);
    setDisplayText(displayText);
}

void MainWindow::onClearClicked()
{
    clearDisplay();
    m_calcHandler->clear();
    m_operatorClicked = false;
    m_resultDisplayed = false;
}

void MainWindow::onClearEntryClicked()
{
    clearDisplay();
    m_resultDisplayed = false;
}

void MainWindow::onCopyClicked()
{
    QString text = getDisplayText();
    if (!text.isEmpty()) {
        QApplication::clipboard()->setText(text);
        UIAnimations::flashSuccess(ui->displayRes, 200);
        qDebug() << "Скопировано в буфер обмена:" << text;
    }
}

void MainWindow::onHistoryClicked()
{
    if (m_historyPanel->isVisible()) {
        m_historyPanel->hide();
        setFixedWidth(360);
    } else {
        m_historyPanel->show();
        setFixedWidth(660);  // 360 + 300
    }
}

void MainWindow::onMemoryAddClicked()
{
    QString displayText = getDisplayText();
    if (displayText.isEmpty()) {
        return;
    }
    
    bool ok = false;
    double value = DisplayFormatter::toDouble(displayText, &ok);
    if (!ok) {
        showError(CalculatorConfig::ERROR_INVALID_INPUT);
        return;
    }
    m_memory->add(value);
    qDebug() << "M+: Добавлено" << value << "к памяти";
}

void MainWindow::onMemorySubtractClicked()
{
    QString displayText = getDisplayText();
    if (displayText.isEmpty()) {
        return;
    }
    
    bool ok = false;
    double value = DisplayFormatter::toDouble(displayText, &ok);
    if (!ok) {
        showError(CalculatorConfig::ERROR_INVALID_INPUT);
        return;
    }
    m_memory->subtract(value);
    qDebug() << "M-: Вычтено" << value << "из памяти";
}

void MainWindow::onMemoryRecallClicked()
{
    if (!m_memory->hasValue()) {
        qDebug() << "MR: Память пуста";
        return;
    }
    
    double value = m_memory->recall();
    QString formattedValue = DisplayFormatter::formatNumber(
        value, CalculatorConfig::MAX_DIGIT_LENGTH);
    setDisplayText(formattedValue);
    
    m_resultDisplayed = true;
    qDebug() << "MR: Вспомнено" << value;
}

void MainWindow::onMemoryClearClicked()
{
    m_memory->clear();
    qDebug() << "MC: Память очищена";
}

void MainWindow::onMemoryStoreClicked()
{
    QString displayText = getDisplayText();
    if (displayText.isEmpty()) {
        return;
    }
    
    bool ok = false;
    double value = DisplayFormatter::toDouble(displayText, &ok);
    if (!ok) {
        showError(CalculatorConfig::ERROR_INVALID_INPUT);
        return;
    }
    m_memory->store(value);
    m_memory->addToList(value);
    UIAnimations::flashSuccess(ui->displayRes, 200);
    qDebug() << "MS: Сохранено в память:" << value;
}

void MainWindow::onMemoryDropdownClicked()
{
    if (m_memory->listSize() == 0) {
        qDebug() << "M˅: Список памяти пуст";
        UIAnimations::shake(ui->displayRes, 300);
        UIAnimations::flashError(ui->displayRes, 200);
        return;
    }
    
    MemoryDropdownDialog dialog(m_memory, this);
    connect(&dialog, &MemoryDropdownDialog::valueSelected,
            this, &MainWindow::onMemoryValueSelected);
    dialog.exec();
}

void MainWindow::onMemoryValueSelected(double value)
{
    QString formattedValue = DisplayFormatter::formatNumber(
        value, CalculatorConfig::MAX_DIGIT_LENGTH);
    setDisplayText(formattedValue);
    m_resultDisplayed = true;
    qDebug() << "Выбрано значение из списка памяти:" << value;
}

void MainWindow::onMemoryChanged(bool hasValue)
{
    // Обновить индикатор памяти в UI (если есть)
    qDebug() << "Память изменена. Есть значение:" << hasValue;
}

void MainWindow::onToggleThemeClicked()
{
    ThemeManager::Theme current = m_themeManager->currentTheme();
    ThemeManager::Theme newTheme = (current == ThemeManager::Theme::Light) 
        ? ThemeManager::Theme::Dark 
        : ThemeManager::Theme::Light;
    
    m_themeManager->setTheme(newTheme);
    qDebug() << "Тема переключена на:" << ThemeManager::themeName(newTheme);
}

void MainWindow::onThemeChanged(ThemeManager::Theme theme)
{
    qDebug() << "Применена тема:" << ThemeManager::themeName(theme);
}

QString MainWindow::getDisplayText() const
{
    return ui->displayRes->text();
}

void MainWindow::setDisplayText(const QString& text)
{
    ui->displayRes->setText(text);
}

void MainWindow::clearDisplay()
{
    ui->displayRes->clear();
}

void MainWindow::showError(const QString& errorMessage)
{
    setDisplayText(errorMessage);
    UIAnimations::flashError(ui->displayRes);
    m_resultDisplayed = true;
}

void MainWindow::applyUnaryOperation(CalcHandler::Operation op)
{
    QString displayText = getDisplayText();
    if (displayText.isEmpty()) {
        return;
    }

    bool ok = false;
    double value = DisplayFormatter::toDouble(displayText, &ok);
    if (!ok) {
        showError(CalculatorConfig::ERROR_INVALID_INPUT);
        return;
    }
    CalcHandler::CalculationResult result =
        m_calcHandler->applyUnaryOperation(op, value);

    if (result.success) {
        QString formattedResult = DisplayFormatter::formatNumber(
            result.value, CalculatorConfig::MAX_DIGIT_LENGTH);
        setDisplayText(formattedResult);
        
        QString operationName = CalcHandler::operationToString(op);
        QString fullExpression = QString("%1(%2) = %3")
            .arg(operationName)
            .arg(displayText)
            .arg(formattedResult);
        m_history->addEntry(fullExpression);
        
        m_operatorClicked = false;
        m_resultDisplayed = true;
    } else {
        showError(result.errorMessage);
        m_operatorClicked = false;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_0:
            handleDigitInput("0");
            break;
        case Qt::Key_1:
            handleDigitInput("1");
            break;
        case Qt::Key_2:
            handleDigitInput("2");
            break;
        case Qt::Key_3:
            handleDigitInput("3");
            break;
        case Qt::Key_4:
            handleDigitInput("4");
            break;
        case Qt::Key_5:
            handleDigitInput("5");
            break;
        case Qt::Key_6:
            handleDigitInput("6");
            break;
        case Qt::Key_7:
            handleDigitInput("7");
            break;
        case Qt::Key_8:
            handleDigitInput("8");
            break;
        case Qt::Key_9:
            handleDigitInput("9");
            break;
        
        case Qt::Key_Plus:
            handleOperatorInput('+');
            break;
        case Qt::Key_Minus:
            handleOperatorInput('-');
            break;
        case Qt::Key_Asterisk:
            handleOperatorInput('*');
            break;
        case Qt::Key_Slash:
            handleOperatorInput('/');
            break;
        
        case Qt::Key_Period:
        case Qt::Key_Comma:
            onDecimalPointClicked();
            break;
        
        case Qt::Key_Enter:
        case Qt::Key_Return:
            onEqualClicked();
            break;
        
        case Qt::Key_Backspace:
            onDeleteClicked();
            break;
        case Qt::Key_Delete:
            onClearClicked();
            break;
        
        case Qt::Key_Percent:
            onPercentClicked();
            break;
        
        case Qt::Key_Escape:
            onClearClicked();
            break;
        
        case Qt::Key_C:
            if (event->modifiers() & Qt::ControlModifier) {
                onCopyClicked();
            }
            break;
        
        case Qt::Key_H:
            if (event->modifiers() & Qt::ControlModifier) {
                onHistoryClicked();
            }
            break;
        
        case Qt::Key_M:
            if (event->modifiers() & Qt::ControlModifier) {
                onMemoryAddClicked();
            }
            break;
        
        case Qt::Key_T:
            if (event->modifiers() & Qt::ControlModifier) {
                onToggleThemeClicked();
            }
            break;
            
        default:
            QMainWindow::keyPressEvent(event);
            break;
    }
}
