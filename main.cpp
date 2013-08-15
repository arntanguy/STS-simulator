#include "mainwindow.h"
#include <QApplication>

#include <QDebug>
#include <iostream>
#include "muParser.h"

#include "function.h"
#include "hierarchicalfunction.h"
#include "unittest.h"
#include  "helperfunctions.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Mainly for QSettings
    QCoreApplication::setOrganizationName("CRANN");
    QCoreApplication::setOrganizationDomain("crann.tcd.ie");
    QCoreApplication::setApplicationName("STS-Simulator");

    mu::Parser *p = HelperFunctions::createParser();
    p->SetExpr("DZ1(10)");
    qDebug() << p->Eval();

    //MainWindow w;
    //w.show();
//    UnitTest test;
//    test.testAll();

    return a.exec();
}
