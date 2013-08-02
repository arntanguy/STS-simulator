#ifndef HIERARCHICALFUNCTION_H
#define HIERARCHICALFUNCTION_H

#include <QList>

#include "function.h"

class HierarchicalFunction : public Function
{
Q_OBJECT
public:
    HierarchicalFunction();
    HierarchicalFunction(int id);

    void addFunction(Function *function);
    void removeFunction(Function *f);
    QList<Function *> getFunctions();

    /**
     * Pure virtual functions
     **/
    virtual QString getExpression() const;
    virtual double compute(double);
    virtual double compute(const QString&, double);
    virtual double operator() (double x)
    {
        return compute(x);
    }

    void loadFromConfig(const QString &group);
    virtual void save(const QString &group);

public Q_SLOTS:
    void update(AbstractFunction *);

private:
    void init();

protected:
    QList<Function *> mFunctions;
};

#endif // HIERARCHICALFUNCTION_H
