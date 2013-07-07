#include "mainwindow.h"
#include <QApplication>

#include <QDebug>
#include <iostream>
#include "muParser.h"

#include "function.h"
#include "hierarchicalfunction.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Mainly for QSettings
    QCoreApplication::setOrganizationName("CRANN");
    QCoreApplication::setOrganizationDomain("crann.tcd.ie");
    QCoreApplication::setApplicationName("STS-Simulator");

    Function f;
    f.setExpression("1*V*f1");
    Function f2;
    f2.setExpression("2*V*f2");
    HierarchicalFunction hf;
    hf.addFunction(&f);
    hf.addFunction(&f2);

    //if(f.isValidExpression())
    //    qDebug() << "valid";
    //else
    //    qDebug() << "invalid";
    try {
        f.setImplicitVariable("f1", 1.d);
        f2.setImplicitVariable("f2", 1.d);
        qDebug() << "f: " << f.getExpression();
        qDebug() << "f(2) = " << f(2);
        qDebug() << "f2: " << f2.getExpression();
        qDebug() << "f2(2) = " << f2(2);
        qDebug() << "hf: " << hf.getExpression();
        qDebug() << "hf(2) = " << hf(2);
    } catch (mu::Parser::exception_type &e)
    {
        std::cout << e.GetMsg() << std::endl;
    }
    MainWindow w;
    w.show();

    return a.exec();
}
