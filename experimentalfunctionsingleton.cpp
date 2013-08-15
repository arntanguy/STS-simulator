#include "singleton.h"
#include "experimentalfunctionsingleton.h"

#include "muParser.h"

#include <QDebug>

// I represents the function index
template<int I> double dz(double x) {
    ExperimentalFunctionSingleton *s = &Singleton<ExperimentalFunctionSingleton>::Instance();
    if(s->hasLinkedData(I)) {
        qDebug() << "dz:: Function "<<I<<" has linked data";
    }
    return s->interpolate(I, x);
}

ExperimentalFunctionSingleton::ExperimentalFunctionSingleton()
{
    for(int i=0; i<NUMBER_OF_DZ; i++) {
        mFunctions[i] = new ExperimentalFunction();
    }
    mFunctions[0]->name = "DZ0";
    mFunctions[0]->function = dz<0>;

    mFunctions[1]->name = "DZ1";
    mFunctions[1]->function = dz<1>;

    mFunctions[2]->name = "DZ2";
    mFunctions[2]->function = dz<2>;

    mFunctions[3]->name = "DZ3";
    mFunctions[3]->function = dz<3>;

    mFunctions[4]->name = "DZ4";
    mFunctions[4]->function = dz<4>;

    mFunctions[5]->name = "DZ5";
    mFunctions[5]->function = dz<5>;

    mFunctions[6]->name = "DZ6";
    mFunctions[6]->function = dz<6>;

    mFunctions[7]->name = "DZ7";
    mFunctions[7]->function = dz<7>;

    mFunctions[8]->name = "DZ8";
    mFunctions[8]->function = dz<8>;

    mFunctions[9]->name = "DZ9";
    mFunctions[9]->function = dz<9>;
}


bool ExperimentalFunctionSingleton::linkData(int dzIndex, PlotData data)
{
    if(dzIndex >= NUMBER_OF_DZ) {
        qDebug() << "ExperimentalFunctionSingleton::linkData -- function doesn't exist for index " << dzIndex;
        return false;
    }
    mFunctions[dzIndex]->data = data;
    return true;
}

bool ExperimentalFunctionSingleton::hasLinkedData(int dzIndex)
{
    if(dzIndex >= NUMBER_OF_DZ) {
        qDebug() << "ExperimentalFunctionSingleton::hasLinkedData -- function doesn't exist for index " << dzIndex;
        return false;
    }
    return (mFunctions[dzIndex]->data.size() == 0);
}

void ExperimentalFunctionSingleton::defineParserFunctions(mu::Parser *parser)
{
    for(int i=0; i<NUMBER_OF_DZ; i++) {
        FuncPtr fun = mFunctions[i]->function;
        parser->DefineFun(mFunctions[i]->name.toStdString(), fun);
    }
}


// Returns the interpolated value for function dzIndex at abscissia x
// XXX: not done!
double ExperimentalFunctionSingleton::interpolate(int dzIndex, double x)
{
    return x;
}
