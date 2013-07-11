#include "hierarchicalfunction.h"
#include <QDebug>

HierarchicalFunction::HierarchicalFunction()
{
    init();
}

void HierarchicalFunction::init()
{
    setType(AbstractFunction::HierarchicalFunction);
}

void HierarchicalFunction::addFunction(AbstractFunction *function)
{
    mFunctions.append(function);
}

void HierarchicalFunction::removeFunction(AbstractFunction *f)
{
    mFunctions.removeAll(f);
}

QList<AbstractFunction *> HierarchicalFunction::getFunctions()
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
    foreach(AbstractFunction *f, mFunctions) {
        result += f->compute(x);
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
    foreach(AbstractFunction *f, mFunctions) {
        exp += f->getExpression() + separator;
    }
    return exp.left(exp.length() - separator.length());
}

void HierarchicalFunction::loadFromConfig(const QString &group)
{
    qDebug() << "HierarchicalFunction::loadFromConfig("<<group<<")";
}

void HierarchicalFunction::save(const QString &group)
{
    qDebug() << "HierarchicalFunction::save - saving function " << mName;
    abstractsave(group);

    QString groupName = group+"/HierarchicalFunction/"+mName;
    qDebug() << "HierarchicalFunction::save - group name: " << groupName;
    qDebug() << "HierarchicalFunction::save - " << mFunctions.size() << " sub-functions to save";
    foreach(AbstractFunction *f, mFunctions) {
        if(f != 0) {
            qDebug() << "HierarchicalFunction::save - saving subfunction " << f->getName();
            f->save(groupName);
        } else {
            qDebug() << "Error saving HierarchicalFunction "<< mName << ": NULL SUBFUNCTION";
        }
    }
}
