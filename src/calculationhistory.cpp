#include "calculationhistory.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

CalculationHistory::CalculationHistory(QObject *parent)
    : QObject(parent)
    , m_maxSize(20)
{
}

void CalculationHistory::addEntry(const QString& expression, double result)
{
    QString entry = QString("%1 = %2").arg(expression).arg(result);
    addEntry(entry);
}

void CalculationHistory::addEntry(const QString& fullExpression)
{
    if (fullExpression.isEmpty()) {
        return;
    }
    
    m_history.prepend(fullExpression);
    
    while (m_history.size() > m_maxSize) {
        m_history.removeLast();
    }
    
    qDebug() << "История добавлена:" << fullExpression;
    emit historyChanged();
}

QStringList CalculationHistory::getAll() const
{
    return m_history;
}

QString CalculationHistory::getLast() const
{
    if (m_history.isEmpty()) {
        return QString();
    }
    return m_history.first();
}

int CalculationHistory::count() const
{
    return m_history.size();
}

void CalculationHistory::clear()
{
    m_history.clear();
    qDebug() << "История очищена";
    emit historyChanged();
}

void CalculationHistory::saveToFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл для записи:" << filename;
        return;
    }
    
    QTextStream out(&file);
    for (const QString& entry : m_history) {
        out << entry << "\n";
    }
    
    file.close();
    qDebug() << "История сохранена в файл:" << filename;
}

void CalculationHistory::loadFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл для чтения:" << filename;
        emit historyChanged();
        return;
    }
    
    m_history.clear();
    
    QStringList lines;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            lines.append(line);
        }
    }
    
    // Добавляем в обратном порядке, чтобы сохранить исходный порядок
    // (т.к. в памяти новые записи в начале через prepend)
    for (int i = lines.size() - 1; i >= 0; --i) {
        m_history.append(lines.at(i));
    }
    
    file.close();
    qDebug() << "История загружена из файла:" << filename << "(" << m_history.size() << "записей)";
    emit historyChanged();
}

void CalculationHistory::setMaxSize(int maxSize)
{
    m_maxSize = maxSize;
    
    while (m_history.size() > m_maxSize) {
        m_history.removeLast();
    }
}

int CalculationHistory::maxSize() const
{
    return m_maxSize;
}
