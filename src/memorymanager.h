#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <QObject>
#include <QList>

// Класс для управления памятью калькулятора (M+, M-, MR, MC, MS, M˅)
class MemoryManager : public QObject
{
    Q_OBJECT

public:
    explicit MemoryManager(QObject *parent = nullptr);
    ~MemoryManager() override = default;

    void add(double value);        // M+ - добавить к памяти
    void subtract(double value);   // M- - вычесть из памяти
    double recall() const;         // MR - вспомнить значение
    void clear();                  // MC - очистить память
    void store(double value);      // MS - сохранить значение (заменить)
    
    bool hasValue() const;         // Есть ли значение в памяти
    double value() const;          // Текущее значение памяти
    
    void addToList(double value);           // Добавить в список памяти
    QList<double> getMemoryList() const;    // Получить список значений
    void recallFromList(int index);         // Вспомнить из списка по индексу
    void removeFromList(int index);         // Удалить из списка
    void clearList();                       // Очистить весь список
    int listSize() const;                   // Размер списка

signals:
    void memoryChanged(bool hasValue);      // Сигнал об изменении основной памяти
    void memoryListChanged(int size);       // Сигнал об изменении списка памяти

private:
    double m_memory;
    QList<double> m_memoryList;
    static const int MAX_MEMORY_ITEMS = 10;
    
    void notifyChange();
    void notifyListChange();
};

#endif // MEMORYMANAGER_H
