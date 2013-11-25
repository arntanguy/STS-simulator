#include "singleton.h"
#include "experimentalfunctionsingleton.h"
#include "projectsingleton.h"

#include "muParser.h"

#include <QSettings>
#include <QDebug>

// I represents the function index
template<int I> double dz(double x) {
    ExperimentalFunctionSingleton *s = &Singleton<ExperimentalFunctionSingleton>::Instance();
    if(s->hasLinkedData(I)) {
        //qDebug() << "dz:: Function "<<I<<" has linked data";
        return s->interpolate(I, x);
    } else {
        //qDebug() << "DZ - id "<<I<<" has NO DATA!";
        return 0;
    }

}

ExperimentalFunctionSingleton::ExperimentalFunctionSingleton()
{
    for(int i=0; i<NUMBER_OF_DZ; i++) {
        mFunctions[i] = new ExperimentalFunction();
    }
    mFunctions[0]->setName("DZ0");
    mFunctions[0]->setFunction(dz<0>);

    mFunctions[1]->setName("DZ1");
    mFunctions[1]->setFunction(dz<1>);

    mFunctions[2]->setName("DZ2");
    mFunctions[2]->setFunction(dz<2>);

    mFunctions[3]->setName("DZ3");
    mFunctions[3]->setFunction(dz<3>);

    mFunctions[4]->setName("DZ4");
    mFunctions[4]->setFunction(dz<4>);

    mFunctions[5]->setName("DZ5");
    mFunctions[5]->setFunction(dz<5>);

    mFunctions[6]->setName("DZ6");
    mFunctions[6]->setFunction(dz<6>);

    mFunctions[7]->setName("DZ7");
    mFunctions[7]->setFunction(dz<7>);

    mFunctions[8]->setName("DZ8");
    mFunctions[8]->setFunction(dz<8>);

    mFunctions[9]->setName("DZ9");
    mFunctions[9]->setFunction(dz<9>);
}


bool ExperimentalFunctionSingleton::linkData(int dzIndex, const QString &experimentId, const QString &abscissiaColumnName, const QString &ordinateColumnName)
{
    if(dzIndex >= NUMBER_OF_DZ) {
        qDebug() << "ExperimentalFunctionSingleton::linkData -- function doesn't exist for index " << dzIndex;
        return false;
    }
    mFunctions[dzIndex]->setData(experimentId, abscissiaColumnName, ordinateColumnName);
    return true;
}

bool ExperimentalFunctionSingleton::hasLinkedData(int dzIndex)
{
    if(dzIndex >= NUMBER_OF_DZ) {
        qDebug() << "ExperimentalFunctionSingleton::hasLinkedData -- function doesn't exist for index " << dzIndex;
        return false;
    }
    return (mFunctions[dzIndex]->hasData());
}

void ExperimentalFunctionSingleton::defineParserFunctions(mu::Parser *parser)
{
    for(int i=0; i<NUMBER_OF_DZ; i++) {
        FuncPtr fun = mFunctions[i]->getFunction();
        parser->DefineFun(mFunctions[i]->getName().toStdString(), fun);
    }
}


// Returns the interpolated value for function dzIndex at abscissia x
// XXX: not done!
double ExperimentalFunctionSingleton::interpolate(int dzIndex, double x)
{
    if(dzIndex >= NUMBER_OF_DZ) {
        qDebug() << "ExperimentalFunctionSingleton::interpolate -- function doesn't exist for index " << dzIndex;
        return false;
    }
    ExperimentalFunction *f = mFunctions[dzIndex];
    if(f->hasData()) {
        return f->interpolate(x);
    }
}

void ExperimentalFunctionSingleton::loadFromSettings()
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("DZ");
    for(int i=0; i<NUMBER_OF_DZ; i++) {
        settings->beginGroup(QString::number(i+1));
        mFunctions[i]->loadFromSettings(settings);
        settings->endGroup();
    }
    settings->endGroup();
}

void ExperimentalFunctionSingleton::save()
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("DZ");
    settings->remove("");
    for(int i=0; i<NUMBER_OF_DZ; i++) {
        settings->beginGroup(QString::number(i+1));
        mFunctions[i]->save(settings);
        settings->endGroup();
    }
    settings->endGroup();
}
