#include "mainwindow.h"
#include <QApplication>

#include <QDebug>
#include <iostream>
#include "muParser.h"

#include "function.h"
#include "hierarchicalfunction.h"
#include "unittest.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Mainly for QSettings
    QCoreApplication::setOrganizationName("CRANN");
    QCoreApplication::setOrganizationDomain("crann.tcd.ie");
    QCoreApplication::setApplicationName("STS-Simulator");

    MainWindow w;
    w.show();

//    UnitTest test;
//    test.testAll();

    return a.exec();
}
