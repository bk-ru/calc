#include "historydialog.h"
#include "calculationhistory.h"
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>

HistoryDialog::HistoryDialog(CalculationHistory* history, QWidget *parent)
    : QDialog(parent)
    , m_history(history)
{
    setupUi();
    loadHistory();
}

void HistoryDialog::setupUi()
{
    setWindowTitle("История вычислений");
    setMinimumSize(400, 500);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    m_countLabel = new QLabel(this);
    mainLayout->addWidget(m_countLabel);
    
    m_listWidget = new QListWidget(this);
    m_listWidget->setAlternatingRowColors(true);
    mainLayout->addWidget(m_listWidget);
    
    QLabel* hintLabel = new QLabel("Двойной клик - копировать в буфер обмена", this);
    hintLabel->setStyleSheet("color: gray; font-size: 10px;");
    mainLayout->addWidget(hintLabel);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    m_clearButton = new QPushButton("Очистить историю", this);
    m_closeButton = new QPushButton("Закрыть", this);
    
    buttonLayout->addWidget(m_clearButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_closeButton);
    
    mainLayout->addLayout(buttonLayout);
    
    connect(m_clearButton, &QPushButton::clicked, this, &HistoryDialog::onClearClicked);
    connect(m_closeButton, &QPushButton::clicked, this, &HistoryDialog::onCloseClicked);
    connect(m_listWidget, &QListWidget::itemDoubleClicked, this, &HistoryDialog::onItemDoubleClicked);
}

void HistoryDialog::loadHistory()
{
    m_listWidget->clear();
    
    QStringList history = m_history->getAll();
    
    for (const QString& entry : history) {
        m_listWidget->addItem(entry);
    }
    
    m_countLabel->setText(QString("Всего записей: %1").arg(history.size()));
    
    if (history.isEmpty()) {
        m_clearButton->setEnabled(false);
    }
}

void HistoryDialog::onClearClicked()
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
        loadHistory();
    }
}

void HistoryDialog::onCloseClicked()
{
    accept();
}

void HistoryDialog::onItemDoubleClicked(QListWidgetItem* item)
{
    if (item) {
        QString text = item->text();
        QApplication::clipboard()->setText(text);
        
        QMessageBox::information(this, "Скопировано", 
            QString("Скопировано в буфер обмена:\n%1").arg(text));
    }
}
