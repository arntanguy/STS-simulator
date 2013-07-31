#include "hierarchicalfunction.h"
#include "projectsingleton.h"
#include "functionssingleton.h"
#include "functionfactory.h"
#include "function.h"

#include <QDebug>
#include <QStringList>

HierarchicalFunction::HierarchicalFunction()
{
    init();
}

void HierarchicalFunction::init()
{
    setType(Function::HierarchicalFunction);
    mBaseGroup = "Functions/HierarchicalFunction/";
}

void HierarchicalFunction::addFunction(Function *function)
{
    function->setGroup(mBaseGroup+mName+"/Function/");
    mFunctions.append(function);
    connect(function, SIGNAL(needsRecompute()), this, SLOT(updateData()));
    connect(function, SIGNAL(curveUpdated(Function *)), this, SLOT(update(Function *)));
}

void HierarchicalFunction::removeFunction(Function *f)
{
    mFunctions.removeAll(f);
}

QList<Function *> HierarchicalFunction::getFunctions()
{
    return mFunctions;
}

// =========  Pure virtual functions =====================
/*!
 * \brief HierarchicalFunction::compute
 * \param x
 *  Abscissia for which value is computed
 * \return
 *  The sum of all functions
 */
double HierarchicalFunction::compute(double x)
{
    double result = 0;
    foreach(Function *f, mFunctions) {
        result += f->compute(x);
    }
    return result;
}

double HierarchicalFunction::compute(const QString& variable, double x)
{
    double result = 0;
    foreach(Function *f, mFunctions) {
        result += f->compute(variable, x);
    }
    return result;
}

/*!
 * \brief HierarchicalFunction::getExpression
 * \return
 *  The concatenated expression of all the sub expressions
 */
QString HierarchicalFunction::getExpression() const
{
    QString exp;
    QString separator = " + ";
    foreach(Function *f, mFunctions) {
        exp += f->getExpression() + separator;
    }
    return exp.left(exp.length() - separator.length());
}

void HierarchicalFunction::loadFromConfig(const QString &group)
{
    qDebug() << "HierarchicalFunction::loadFromConfig("<<group<<")";
    Function::loadFromConfig(group);
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup(group);

    setName(settings->value("name", "Unknown").toString());
    settings->beginGroup("Function/");
    QStringList groups = settings->childGroups();
    settings->endGroup();

    settings->endGroup();

    foreach(QString fName, groups) {
        qDebug() << "HierarchicalFunction::loadFromConfig() - adding function " << fName << " to " << mName;
        Function *f = FunctionFactory::createFromConfig(group+"/Function/"+fName);
        if(f != 0) {
            addFunction(f);
        } else {
            qDebug() << "FATAL Error: function " << fName << " doesn't exist!";
        }
    }
}

void HierarchicalFunction::save(const QString &group)
{
    qDebug() << "HierarchicalFunction::save - saving function " << mName;
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    AbstractFunction::save(group+"/HierarchicalFunction/");

    QString groupName = group+"/HierarchicalFunction/"+mName;
    settings->beginGroup(groupName);
    qDebug() << "HierarchicalFunction::save - group name: " << groupName;
    qDebug() << "HierarchicalFunction::save - " << mFunctions.size() << " sub-functions to save";
    foreach(Function *f, mFunctions) {
        if(f != 0) {
            qDebug() << "HierarchicalFunction::save - saving subfunction " << f->getName();
            f->save("");
        } else {
            qDebug() << "Error saving HierarchicalFunction "<< mName << ": NULL SUBFUNCTION";
        }
    }
    settings->endGroup();
}


// =============================== SLOTS ========================================
void HierarchicalFunction::update(Function *f)
{
    updateLinkedCurve();
}
