#include "unittest.h"
#include "function.h"
#include "hierarchicalfunction.h"

#include <QStringList>
#include <QDebug>

UnitTest::UnitTest()
{
}

void UnitTest::testComputeSimpleFunction()
{
    qDebug() << "===============================";
    qDebug() << "UnitTest: testing Function class";
    qDebug() << "===============================\n";

    Function f1 = new Function();
    f1.setName("f1");
    f1.setExpression("2*bob*alice*V");
    f1.compute(0);
    qDebug() << "Setting expression " << f1.getExpression() << " to f1";
    qDebug() << "Listing implicit variables. Result should be alice, bob";
    f1.listAllVariables();

    qDebug() << "Setting variable alice to 3";
    qDebug() << "Setting variable bob to 2";
    f1.setImplicitVariable("alice", 3);
    f1.setImplicitVariable("bob", 2);
    qDebug() << "Computing test";
    double f1_1 = f1(2);
    double f1_2 = f1(243);
    double f1_3 = f1(2333345);
    qDebug() << "f1(2)=" << f1_1 << ((f1_1 == 24) ? "ok" : "error, expected 24");
    qDebug() << "f1(243)=" << f1_2 << ((f1_2 == 2916) ? "ok" : "error, expected 2916");
    qDebug() << "f1(2333345)=" << f1_3 << ((f1_3 == 28000140) ? "ok" : "error, expected 28000140");
}

void UnitTest::testCopyFunction()
{
    qDebug() << "===================";
    qDebug() << "Test Copy function";
    qDebug() << "===================\n";

    Function f1 = new Function();
    f1.setName("f1");
    f1.setExpression("bob");
    f1.setImplicitVariable("bob", 0);

    Function f2(f1);

    double f1_0 = f1.compute(0);
    double f2_0 = f2.compute(0);

    qDebug() << "test f1=f2 " << ((f1_0 == f2_0) ? "ok" : "error");

    qDebug() << "set f1 variable bob to 1, leave f2 to 0";
    f1.setImplicitVariable("bob", 1);
    double f1_1 = f1.compute(0);
    double f2_1 = f2.compute(0);
    qDebug() << "compute f1 with bob=1: " << ((f1_1 == 1) ? "ok": "error");
    qDebug() << "ensure that f2 variable wasn't modified (f2_1 = f2_0) : " << ((f2_1 == f2_0) ? "ok" : "error");
}

void UnitTest::testAll()
{
    qDebug() << "===============================";
    qDebug() << "UnitTest: testing all functions";
    qDebug() << "===============================\n\n";

    testComputeSimpleFunction();
    testCopyFunction();
}
