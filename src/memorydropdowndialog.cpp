#include "memorydropdowndialog.h"
#include "displayformatter.h"
#include "calculatorconfig.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

MemoryDropdownDialog::MemoryDropdownDialog(MemoryManager* memory, QWidget* parent)
    : QDialog(parent)
    , m_memory(memory)
    , m_listWidget(new QListWidget(this))
    , m_clearAllButton(new QPushButton("Очистить всё", this))
    , m_deleteButton(new QPushButton("Удалить", this))
{
    setupUi();
    connectSignals();
    updateList();
}

void MemoryDropdownDialog::setupUi()
{
    setWindowTitle("Список памяти");
    setMinimumSize(300, 400);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QLabel* titleLabel = new QLabel("Сохраненные значения:", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 12pt;");
    mainLayout->addWidget(titleLabel);
    
    m_listWidget->setAlternatingRowColors(true);
    m_listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    mainLayout->addWidget(m_listWidget);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_clearAllButton);
    
    mainLayout->addLayout(buttonLayout);
    
    m_deleteButton->setEnabled(false);
}

void MemoryDropdownDialog::connectSignals()
{
    connect(m_listWidget, &QListWidget::itemDoubleClicked,
            this, &MemoryDropdownDialog::onItemDoubleClicked);
    
    connect(m_listWidget, &QListWidget::itemSelectionChanged,
            this, [this]() {
                m_deleteButton->setEnabled(m_listWidget->currentRow() >= 0);
            });
    
    connect(m_clearAllButton, &QPushButton::clicked,
            this, &MemoryDropdownDialog::onClearAllClicked);
    
    connect(m_deleteButton, &QPushButton::clicked,
            this, &MemoryDropdownDialog::onDeleteClicked);
    
    connect(m_memory, &MemoryManager::memoryListChanged,
            this, &MemoryDropdownDialog::updateList);
}

void MemoryDropdownDialog::onItemDoubleClicked(QListWidgetItem* item)
{
    if (!item) return;
    
    int row = m_listWidget->row(item);
    QList<double> memoryList = m_memory->getMemoryList();
    
    if (row >= 0 && row < memoryList.size()) {
        double value = memoryList.at(row);
        qDebug() << "Выбрано значение из списка:" << value;
        emit valueSelected(value);
        accept();
    }
}

void MemoryDropdownDialog::onClearAllClicked()
{
    m_memory->clearList();
    qDebug() << "Весь список памяти очищен";
}

void MemoryDropdownDialog::onDeleteClicked()
{
    int currentRow = m_listWidget->currentRow();
    if (currentRow >= 0) {
        m_memory->removeFromList(currentRow);
        qDebug() << "Удален элемент из списка:" << currentRow;
    }
}

void MemoryDropdownDialog::updateList()
{
    m_listWidget->clear();
    
    QList<double> memoryList = m_memory->getMemoryList();
    
    if (memoryList.isEmpty()) {
        QListWidgetItem* emptyItem = new QListWidgetItem("Список пуст");
        emptyItem->setFlags(Qt::NoItemFlags);
        emptyItem->setForeground(Qt::gray);
        m_listWidget->addItem(emptyItem);
        m_clearAllButton->setEnabled(false);
        m_deleteButton->setEnabled(false);
        return;
    }
    
    m_clearAllButton->setEnabled(true);
    
    for (const double& value : memoryList) {
        QString formattedValue = DisplayFormatter::formatNumber(
            value, CalculatorConfig::MAX_DIGIT_LENGTH);
        QListWidgetItem* item = new QListWidgetItem(formattedValue);
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_listWidget->addItem(item);
    }
    
    qDebug() << "Список памяти обновлен. Элементов:" << memoryList.size();
}
