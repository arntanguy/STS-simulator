#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    // Mainly for QSettings
    QCoreApplication::setOrganizationName("CRANN");
    QCoreApplication::setOrganizationDomain("crann.tcd.ie");
    QCoreApplication::setApplicationName("STS-Simulator");

    MainWindow w;
    w.show();
    
    return a.exec();
}
