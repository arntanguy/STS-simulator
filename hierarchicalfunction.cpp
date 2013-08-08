#include "hierarchicalfunction.h"
#include "projectsingleton.h"
#include "functionssingleton.h"
#include "functionfactory.h"
#include "function.h"

#include <QDebug>
#include <QStringList>

HierarchicalFunction::HierarchicalFunction() : Function()
{
    init();
}

HierarchicalFunction::HierarchicalFunction(int id) : Function(id)
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
    qDebug() << "HierarchicalFunction::addFunction()";
    //function->setGroup(mBaseGroup+mName+"/Function/");
    mFunctions.append(function);
    connect(function, SIGNAL(functionUpdated(AbstractFunction *)), this, SLOT(update(AbstractFunction *)));
    connect(function, SIGNAL(expressionChanged()), this, SLOT(update()));
    emit expressionChanged();
}

void HierarchicalFunction::removeFunction(Function *f)
{
    mFunctions.removeAll(f);
    emit expressionChanged();
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
    qDebug() << "HierarchicalFunction::loadFromConfig("<<group<<", id: " << getId()<<")";
    Function::loadFromConfig(group);

    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup(group+"/Function");
    QStringList groups = settings->childGroups();
    settings->endGroup();

    foreach(QString fId, groups) {
        Function *f = FunctionFactory::createFromConfig(group+"/Function/"+fId);
        if(f != 0) {
            addFunction(f);
        } else {
            qDebug() << "FATAL Error: function " << fId << " doesn't exist!";
        }
    }
}

void HierarchicalFunction::save(const QString &group)
{
    qDebug() << "HierarchicalFunction::save - saving function " << mName;
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    AbstractFunction::abstractsave(group+"/HierarchicalFunction/");

    QString groupName = group+"/HierarchicalFunction/"+QString::number(getId());
    foreach(Function *f, mFunctions) {
        if(f != 0) {
            f->save(groupName);
        } else {
            qDebug() << "Error saving HierarchicalFunction "<< mName << ": NULL SUBFUNCTION";
        }
    }
}


// =============================== SLOTS ========================================
void HierarchicalFunction::update(AbstractFunction *f)
{
    update();
}

void HierarchicalFunction::update()
{
    qDebug()<< "HierarchicalFunction::update()";
    updateLinkedCurve(true);
    emit functionUpdated(this);
}
