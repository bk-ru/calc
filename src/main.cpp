#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Установить иконку приложения
    a.setWindowIcon(QIcon(":/docs/icon.png"));
    
    MainWindow w;
    w.show();
    return a.exec();
}
