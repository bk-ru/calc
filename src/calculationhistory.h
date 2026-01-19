#ifndef CALCULATIONHISTORY_H
#define CALCULATIONHISTORY_H

#include <QObject>
#include <QStringList>
#include <QString>

// Класс для управления историей вычислений
class CalculationHistory : public QObject
{
    Q_OBJECT

public:
    explicit CalculationHistory(QObject *parent = nullptr);
    ~CalculationHistory() override = default;

    void addEntry(const QString& expression, double result);
    void addEntry(const QString& fullExpression);
    QStringList getAll() const;
    QString getLast() const;
    int count() const;
    void clear();
    
    void saveToFile(const QString& filename);
    void loadFromFile(const QString& filename);
    
    void setMaxSize(int maxSize);
    int maxSize() const;

signals:
    void historyChanged();

private:
    QStringList m_history;
    int m_maxSize;
};

#endif // CALCULATIONHISTORY_H
