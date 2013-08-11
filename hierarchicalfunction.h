#ifndef HIERARCHICALFUNCTION_H
#define HIERARCHICALFUNCTION_H

#include <QList>
#include <QSharedPointer>
#include "typedefs.h"

#include "function.h"

class HierarchicalFunction : public Function
{
Q_OBJECT
public:
    HierarchicalFunction();
    HierarchicalFunction(int id);
    virtual ~HierarchicalFunction();

    void addFunction(const QSharedPointer<Function>& function);
    void removeFunction(const QSharedPointer<Function> &f);
    QList<FunctionPtr> getFunctions();

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
    void update();

private:
    void init();

protected:
    QList<FunctionPtr> mFunctions;
};

Q_DECLARE_METATYPE(HierarchicalFunctionPtr);

#endif // HIERARCHICALFUNCTION_H
