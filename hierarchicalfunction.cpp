#include "hierarchicalfunction.h"
#include <QDebug>

HierarchicalFunction::HierarchicalFunction()
{
}


void HierarchicalFunction::addFunction(AbstractFunction *function)
{
    mFunctions.append(function);
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

void HierarchicalFunction::save(const QString &group)
{
    qDebug() << "HierarchicalFunction::save - saving function " << mName;
    abstractsave(group);

    QString groupName = group+"/"+mName;
    qDebug() << "HierarchicalFunction::save - group name: " << groupName;
    qDebug() << "HierarchicalFunction::save - " << mFunctions.size() << " sub-functions to save";
    AbstractFunction *f;
    for(int i = 0; i<mFunctions.length(); i++) {
        qDebug() << "HierarchicalFunction::save - saving subfunction " << i;
        f = 0;
        f = mFunctions[i];
        if(f != 0) {
            qDebug() << "non null function: " << f;
            qDebug() << "HierarchicalFunction::save - saving subfunction " << f->getName();
            f->save(groupName);
        } else {
            qDebug() << "Error saving HierarchicalFunction "<< mName << ": NULL SUBFUNCTION";
        }
    }
}
