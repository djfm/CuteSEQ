#include <QtGui/QApplication>

#include "mainwindow.h"
#include "jackpp.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    JackPP::server();
    
    return a.exec();
}
