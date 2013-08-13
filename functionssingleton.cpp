#include "functionssingleton.h"
#include "projectsingleton.h"
#include "function.h"
#include "hierarchicalfunction.h"
#include "integralfunction.h"
#include "abstractfunction.h"
#include "differentialfunction.h"
#include "curve.h"
#include "functioncurve.h"
#include "curvesingleton.h"
#include "csvfilewriter.h"

#include <QStringList>
#include <QDebug>

FunctionsSingleton::FunctionsSingleton()
{
}
FunctionsSingleton::~FunctionsSingleton()
{
    clear();
}

void FunctionsSingleton::clear()
{
    qDebug() << "FunctionsSingleton::clear()";
    //qDeleteAll(mFunctions);
    mFunctions.clear();
    AbstractFunction::resetStaticId();
}

void FunctionsSingleton::addFunction(const AbstractFunctionPtr &f)
{
    qDebug() << "FunctionSingleton::addFunction: adding function "<<f->getName();
    if(f != 0) {
        QSharedPointer<AbstractFunction> pf(f);
        mFunctions[f->getId()] = pf;
    } else {
        qDebug() << "FunctionSingleton::addFunction: CANNOT ADD A NULL FUNCTION";
    }
}

IntegralFunctionPtr FunctionsSingleton::isSubFunctionOfIntegral(const AbstractFunctionPtr &f)
{
    foreach(AbstractFunctionPtr ptr, mFunctions) {
        if(ptr->getType() == AbstractFunction::Integral) {
            IntegralFunctionPtr intPtr = qSharedPointerDynamicCast<IntegralFunction>(ptr);
            if(intPtr != 0 && intPtr->hasFunction(f->getId())) {
                return intPtr;
            }
        }
    }
    return IntegralFunctionPtr();
}

bool  FunctionsSingleton::removeFunction(const AbstractFunctionPtr &f)
{
    qDebug() << "FunctionsSingleton::removeFunction";
    bool canDelete = false;
    if(f != 0) {
        canDelete = true;
        QMap< int, QSharedPointer<AbstractFunction> >::iterator it = mFunctions.find(f->getId());
        if(it != mFunctions.end()) {
            if(f != 0) {
                // Remove base function.
                // Harmless since everything is a copy of that
                if(f->getType() == AbstractFunction::Function) {
                    qDebug() << "FunctionsSingleton::removeFunction() - deleting " << it.value()->getName();
                    canDelete = true;
                }
                // Removes differential function.
                // Can remove the integral function associated. Managed by QSharedPointers so instance won't be deleted as long as another function is using it
                else if(f->getType() == AbstractFunction::Differential) {
                    qDebug() << "FunctionsSingleton::removeFunction() -- DIFFERENTIAL";
                    canDelete = true;
                }

                // Integral function can be deleted safely
                else if(f->getType() == AbstractFunction::Integral) {
                    qDebug() << "FunctionsSingleton::removeFunction() -- INTEGRAL";
                    canDelete = true;
                }

                // Hierarchicalfunction is tricky: if deleted it should be deleted from the integral.
                // If not then, save/load would fail, the integral wouldn't be controllable anymore!
                // Delete only if not linked, else do nothing, return false
                else if(f->getType() == AbstractFunction::HierarchicalFunction) {
                    qDebug() << "FunctionsSingleton::removeFunction() -- HIERARCHICAL";
                    canDelete = true;
                    foreach(AbstractFunctionPtr af, mFunctions) {
                        if(af->getType() == AbstractFunction::Integral) {
                            IntegralFunctionPtr intPtr = qSharedPointerDynamicCast<IntegralFunction>(af);
                            if(intPtr->hasFunction(f->getId())) {
                                qDebug() << "Hierarchical Function " << f->getName() << " is referenced by integral " << intPtr->getName() << ": can't delete";
                                canDelete = false;
                                break;
                            }
                        }
                    }
                }             }
        }
        if(canDelete) {
            qDebug() << "deleting function " << f->getName();
            Singleton<CurveSingleton>::Instance().removeCurve(f->getCurve());
            mFunctions.erase(it);
        }
    }
    return canDelete;
}

AbstractFunctionPtr FunctionsSingleton::getFunctionById(int id)
{
    foreach(QSharedPointer<AbstractFunction> f, mFunctions) {
        if(f->getId() == id) {
            return f;
        }
    }
    return AbstractFunctionPtr(0);
}

QList<int> FunctionsSingleton::getFunctionIds() const
{
    return mFunctions.keys();
}

bool FunctionsSingleton::functionNameExists(const QString& name) const
{
    foreach(AbstractFunctionPtr f, mFunctions) {
        if(f != 0) {
            if(f->getName() == name) {
                return true;
            }
        }
    }
    return false;
}

void FunctionsSingleton::loadFromSettings()
{
    qDebug() << "FunctionsSingleton::loadFromSettings()";
	QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();

    settings->beginGroup("Functions/Function/");
    QStringList groups = settings->childGroups();
    settings->endGroup();

    foreach(QString functionId, groups) {
        FunctionPtr f(new Function(functionId.toInt()));
        f->loadFromConfig("Functions/Function/"+functionId);
        addFunction(f);
    }

    settings->beginGroup("Functions/HierarchicalFunction/");
    QStringList hGroups = settings->childGroups();
    settings->endGroup();

    foreach(QString functionId, hGroups) {
        HierarchicalFunctionPtr f(new HierarchicalFunction(functionId.toInt()));
        f->loadFromConfig("Functions/HierarchicalFunction/"+functionId);
        addFunction(f);
    }

    settings->beginGroup("Functions/IntegralFunction/");
    QStringList iGroups = settings->childGroups();
    settings->endGroup();

    foreach(QString functionId, iGroups) {
        IntegralFunctionPtr f(new IntegralFunction(functionId.toInt()));
        f->loadFromConfig("Functions/IntegralFunction/"+functionId);
        addFunction(f);
    }

    settings->beginGroup("Functions/DifferentialFunction/");
    QStringList dGroups = settings->childGroups();
    settings->endGroup();
    foreach(QString functionId, dGroups) {
        DifferentialFunctionPtr f(new DifferentialFunction(functionId.toInt()));
        f->loadFromConfig("Functions/DifferentialFunction/"+functionId);
        addFunction(f);
    }

    foreach(QSharedPointer<AbstractFunction> f, mFunctions) {
        f->updateLinkedCurve(true);
    }
}

void FunctionsSingleton::save()
{
    qDebug() << "FunctionsSingleton::save() : Saving all functions...";
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();

    // First cleanup the curves
    settings->beginGroup("Functions");
    settings->remove("");
    settings->endGroup();


    // Now save all the curves
    foreach(int fId, mFunctions.keys()) {
        qDebug() << "Function " << fId;
        mFunctions[fId]->save("Functions");
    }
}

bool FunctionsSingleton::exportVariables(const QString& path)
{
    CSVFileWriter w("\t");
    if(w.openFile(path)) {
        QString fName, name, type;

        foreach(AbstractFunctionPtr af, mFunctions) {
            int fNumber = 0;
            if(af->getType() == AbstractFunction::HierarchicalFunction) {
                HierarchicalFunctionPtr hf = qSharedPointerDynamicCast<HierarchicalFunction>(af);
                if(hf != 0) {
                    fName = af->getName();
                    foreach(FunctionPtr f, hf->getFunctions()) {
                        name = fName+"_"+QString::number(fNumber++);
                        type = f->getName();
                        QStringList line;
                        line << name << type << f->getVariableList();
                        w.writeLine(line);
                        line.clear();
                        line << name << type << f->getVariablesValueList();
                        w.writeLine(line);
                    }
                }
            }
        }
    }
}
