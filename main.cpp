#include "mainwindow.h"
#include <QApplication>

#include <QDebug>
#include <iostream>
#include "muParser.h"
// Function callback
double MySqr(double a_fVal)
{
  return a_fVal*a_fVal;
}

// main program
//int main(int argc, char* argv[])
//{
//  using namespace mu;
//
//  try
//  {
//    double fVal = 1;
//    Parser p;
//    p.DefineVar("a", &fVal);
//    p.DefineFun("MySqr", MySqr);
//    p.SetExpr("MySqr(a)\n*_pi+min(10,a)");
//
//    /**
//     * Queries all used variable in expression
//     **/
//    {
//        // Get the map with the variables
//        varmap_type variables = p.GetVar();
//        std::cout << "Number: " << (int)variables.size() << "\n";
//
//        // Get the number of variables
//        varmap_type::const_iterator item = variables.begin();
//
//        // Query the variables
//        for (; item!=variables.end(); ++item)
//        {
//            std::cout << "Name: " << item->first << " Address: [0x" << item->second << "]\n";
//        }
//    }
//
//
//    /**
//     * Querying parser constants
//     **/
//    {
//        valmap_type cmap = p.GetConst();
//        if (cmap.size())
//        {
//            valmap_type::const_iterator item = cmap.begin();
//            for (; item!=cmap.end(); ++item)
//                std::cout << "  " << item->first << " =  " << item->second << "\n";
//        }
//    }
//
//    for (std::size_t a=0; a<100; ++a)
//    {
//      fVal = a;  // Change value of variable a
//      std::cout << p.Eval() << std::endl;
//    }
//  }
//  catch (Parser::exception_type &e)
//  {
//    std::cout << e.GetMsg() << std::endl;
//  }
//  return 0;
//}
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
