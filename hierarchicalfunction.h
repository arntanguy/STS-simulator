#ifndef HIERARCHICALFUNCTION_H
#define HIERARCHICALFUNCTION_H

#include <QList>

#include "abstractfunction.h"

class HierarchicalFunction : public AbstractFunction
{
public:
    HierarchicalFunction();

    void addFunction(AbstractFunction *function);
    void removeFunction(AbstractFunction *f);
    QList<AbstractFunction *> getFunctions();

    /**
     * Pure virtual functions
     **/
    virtual QString getExpression() const;
    virtual double compute(double);
    virtual double operator() (double x)
    {
        return compute(x);
    }

    void loadFromConfig(const QString &group);
    virtual void save(const QString &group);

private:
    void init();

private:
    QList<AbstractFunction *> mFunctions;
};

#endif // HIERARCHICALFUNCTION_H
