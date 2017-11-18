#include "mainwindow.h"
#include <QApplication>
#include <qapplication.h>
//#include "samplingthread.h"
//#include "mainwindow.h"
#include "ui_mainwindow.h"

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    //*******************
    QApplication app( argc, argv );
    app.setWindowIcon(QIcon("AriaElec.icon"));

    MainWindow w;


    w.show();


    w.start();

    bool ok = app.exec();

    return ok;
    //*******************


//    return a.exec();
}
