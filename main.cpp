#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.MakeClientWithUrl("https://restcountries.com/v3.1/all?fields=name,capital,region,subregion,area,population");
    w.show();
    return a.exec();
}
