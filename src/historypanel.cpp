#include "historypanel.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>
#include <QClipboard>

// HistoryItemWidget - виджет для одной записи
HistoryItemWidget::HistoryItemWidget(const QString& expression, const QString& result, QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(15, 10, 15, 10);
    layout->setSpacing(5);
    
    m_expressionLabel = new QLabel(expression, this);
    m_expressionLabel->setAlignment(Qt::AlignRight);
    m_expressionLabel->setStyleSheet("color: #888; font-size: 12pt;");
    
    m_resultLabel = new QLabel(result, this);
    m_resultLabel->setAlignment(Qt::AlignRight);
    m_resultLabel->setStyleSheet("font-size: 18pt; font-weight: bold;");
    
    layout->addWidget(m_expressionLabel);
    layout->addWidget(m_resultLabel);
    
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("HistoryItemWidget { background-color: transparent; border-radius: 5px; }");
}

void HistoryItemWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
    QWidget::mousePressEvent(event);
}

void HistoryItemWidget::enterEvent(QEvent* event)
{
    setStyleSheet("HistoryItemWidget { background-color: rgba(128, 128, 128, 0.1); border-radius: 5px; }");
    QWidget::enterEvent(event);
}

void HistoryItemWidget::leaveEvent(QEvent* event)
{
    setStyleSheet("HistoryItemWidget { background-color: transparent; border-radius: 5px; }");
    QWidget::leaveEvent(event);
}

// HistoryPanel - боковая панель
HistoryPanel::HistoryPanel(CalculationHistory* history, QWidget* parent)
    : QWidget(parent)
    , m_history(history)
    , m_isVisible(false)
{
    setupUi();
    setFixedWidth(300);
    hide();
    
    // Автоматически обновлять панель при изменении истории
    connect(m_history, &CalculationHistory::historyChanged,
            this, &HistoryPanel::updateHistory);
}

void HistoryPanel::setupUi()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(10, 10, 10, 10);
    m_mainLayout->setSpacing(10);
    
    // Заголовок
    m_titleLabel = new QLabel("История", this);
    m_titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold;");
    m_mainLayout->addWidget(m_titleLabel);
    
    // Область прокрутки
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    
    m_scrollContent = new QWidget();
    m_scrollLayout = new QVBoxLayout(m_scrollContent);
    m_scrollLayout->setContentsMargins(0, 0, 0, 0);
    m_scrollLayout->setSpacing(5);
    m_scrollLayout->addStretch();
    
    m_scrollArea->setWidget(m_scrollContent);
    m_mainLayout->addWidget(m_scrollArea);
    
    // Надпись "Пусто"
    m_emptyLabel = new QLabel("История пуста", this);
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->setStyleSheet("color: #888; font-size: 12pt; padding: 20px;");
    m_mainLayout->addWidget(m_emptyLabel);
    
    // Кнопка очистки
    m_clearButton = new QPushButton("🗑 Очистить историю", this);
    m_clearButton->setMinimumHeight(40);
    m_mainLayout->addWidget(m_clearButton);
    
    connect(m_clearButton, &QPushButton::clicked, this, &HistoryPanel::onClearClicked);
    
    updateHistory();
}

void HistoryPanel::updateHistory()
{
    // Очистить текущие элементы
    QLayoutItem* item;
    while ((item = m_scrollLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
    
    QStringList history = m_history->getAll();
    
    if (history.isEmpty()) {
        m_emptyLabel->show();
        m_scrollArea->hide();
        m_clearButton->setEnabled(false);
    } else {
        m_emptyLabel->hide();
        m_scrollArea->show();
        m_clearButton->setEnabled(true);
        
        // Добавить элементы истории (в обратном порядке - новые сверху)
        for (int i = history.size() - 1; i >= 0; --i) {
            addHistoryItem(history.at(i));
        }
    }
    
    m_scrollLayout->addStretch();
}

void HistoryPanel::addHistoryItem(const QString& fullExpression)
{
    QString expression, result;
    parseExpression(fullExpression, expression, result);
    
    HistoryItemWidget* item = new HistoryItemWidget(expression, result, m_scrollContent);
    
    connect(item, &HistoryItemWidget::clicked, this, [result]() {
        QApplication::clipboard()->setText(result);
    });
    
    m_scrollLayout->insertWidget(0, item);
}

void HistoryPanel::parseExpression(const QString& fullExpression, QString& expression, QString& result)
{
    // Формат: "5 + 3 = 8" или "√(16) = 4"
    int equalPos = fullExpression.lastIndexOf(" = ");
    
    if (equalPos != -1) {
        expression = fullExpression.left(equalPos);
        result = fullExpression.mid(equalPos + 3);
    } else {
        expression = fullExpression;
        result = "";
    }
}

void HistoryPanel::show()
{
    m_isVisible = true;
    updateHistory();
    QWidget::show();
}

void HistoryPanel::hide()
{
    m_isVisible = false;
    QWidget::hide();
    emit closed();
}

bool HistoryPanel::isVisible() const
{
    return m_isVisible;
}

void HistoryPanel::onClearClicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Подтверждение");
    msgBox.setText("Вы уверены, что хотите очистить всю историю?");
    msgBox.setIcon(QMessageBox::Question);
    
    QPushButton* yesButton = msgBox.addButton("Да", QMessageBox::YesRole);
    QPushButton* noButton = msgBox.addButton("Нет", QMessageBox::NoRole);
    msgBox.setDefaultButton(noButton);
    
    msgBox.exec();
    
    if (msgBox.clickedButton() == yesButton) {
        m_history->clear();
        updateHistory();
    }
}
