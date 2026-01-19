#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class CalculationHistory;

// Диалоговое окно для отображения истории вычислений
class HistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryDialog(CalculationHistory* history, QWidget *parent = nullptr);
    ~HistoryDialog() override = default;

private slots:
    void onClearClicked();
    void onCloseClicked();
    void onItemDoubleClicked(QListWidgetItem* item);

private:
    void setupUi();
    void loadHistory();

private:
    CalculationHistory* m_history;
    QListWidget* m_listWidget;
    QPushButton* m_clearButton;
    QPushButton* m_closeButton;
    QLabel* m_countLabel;
};

#endif // HISTORYDIALOG_H
