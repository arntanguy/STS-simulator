#include "hierarchicalfunction.h"

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
