#ifndef MEMORYDROPDOWNDIALOG_H
#define MEMORYDROPDOWNDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "memorymanager.h"

// Диалог для отображения списка сохраненных значений в памяти (M˅)
class MemoryDropdownDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MemoryDropdownDialog(MemoryManager* memory, QWidget* parent = nullptr);
    ~MemoryDropdownDialog() override = default;

signals:
    void valueSelected(double value);

private slots:
    void onItemDoubleClicked(QListWidgetItem* item);
    void onClearAllClicked();
    void onDeleteClicked();
    void updateList();

private:
    MemoryManager* m_memory;
    QListWidget* m_listWidget;
    QPushButton* m_clearAllButton;
    QPushButton* m_deleteButton;
    
    void setupUi();
    void connectSignals();
};

#endif // MEMORYDROPDOWNDIALOG_H
