#include <QtGui/QApplication>

#include "mainwindow.h"
#include "jackpp.h"
#include "alsapp.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //JackPP::server();
    AlsaPP::server();
    
    return a.exec();
}
