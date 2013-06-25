#include "mainwindow.h"
#include <QApplication>

#include <QDebug>
#include <iostream>
#include "muParser.h"

#include "function.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Mainly for QSettings
    QCoreApplication::setOrganizationName("CRANN");
    QCoreApplication::setOrganizationDomain("crann.tcd.ie");
    QCoreApplication::setApplicationName("STS-Simulator");

    Function f;
    f.setExpression("2*V*a*b");
    if(f.isValidExpression())
        qDebug() << "valid";
    else
        qDebug() << "invalid";
    try {
        qDebug() << f(4);
        f.setImplicitVariable("a", 1.d);
        f.setImplicitVariable("b", 1.d);
        qDebug() << f(4);
    } catch (mu::Parser::exception_type &e)
    {
        std::cout << e.GetMsg() << std::endl;
    }
    MainWindow w;
    w.show();

    return a.exec();
}
