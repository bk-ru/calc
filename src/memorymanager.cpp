#include "memorymanager.h"
#include <QDebug>
#include <cmath>

MemoryManager::MemoryManager(QObject *parent)
    : QObject(parent)
    , m_memory(0.0)
{
}

void MemoryManager::add(double value)
{
    m_memory += value;
    qDebug() << "M+:" << value << "-> Память:" << m_memory;
    notifyChange();
}

void MemoryManager::subtract(double value)
{
    m_memory -= value;
    qDebug() << "M-:" << value << "-> Память:" << m_memory;
    notifyChange();
}

double MemoryManager::recall() const
{
    qDebug() << "MR: Вспомнить" << m_memory;
    return m_memory;
}

void MemoryManager::clear()
{
    m_memory = 0.0;
    qDebug() << "MC: Память очищена";
    notifyChange();
}

void MemoryManager::store(double value)
{
    m_memory = value;
    qDebug() << "MS: Сохранено в память:" << value;
    notifyChange();
}

bool MemoryManager::hasValue() const
{
    return !qFuzzyIsNull(m_memory);
}

double MemoryManager::value() const
{
    return m_memory;
}

void MemoryManager::addToList(double value)
{
    if (m_memoryList.size() >= MAX_MEMORY_ITEMS) {
        m_memoryList.removeLast();
        qDebug() << "Список памяти полон, удален последний элемент";
    }
    
    m_memoryList.prepend(value);
    qDebug() << "Добавлено в список памяти:" << value << "Размер списка:" << m_memoryList.size();
    notifyListChange();
}

QList<double> MemoryManager::getMemoryList() const
{
    return m_memoryList;
}

void MemoryManager::recallFromList(int index)
{
    if (index >= 0 && index < m_memoryList.size()) {
        m_memory = m_memoryList.at(index);
        qDebug() << "Вспомнить из списка [" << index << "]:" << m_memory;
        notifyChange();
    } else {
        qDebug() << "Ошибка: неверный индекс" << index;
    }
}

void MemoryManager::removeFromList(int index)
{
    if (index >= 0 && index < m_memoryList.size()) {
        double removed = m_memoryList.at(index);
        m_memoryList.removeAt(index);
        qDebug() << "Удалено из списка [" << index << "]:" << removed;
        notifyListChange();
    } else {
        qDebug() << "Ошибка: неверный индекс" << index;
    }
}

void MemoryManager::clearList()
{
    m_memoryList.clear();
    qDebug() << "Список памяти очищен";
    notifyListChange();
}

int MemoryManager::listSize() const
{
    return m_memoryList.size();
}

void MemoryManager::notifyChange()
{
    emit memoryChanged(hasValue());
}

void MemoryManager::notifyListChange()
{
    emit memoryListChanged(m_memoryList.size());
}
