#ifndef ABSTRACTFUNCTION_H
#define ABSTRACTFUNCTION_H

#include <QString>
#include <QMetaType>

class AbstractFunction
{
public:
    enum FunctionType { Function, HierarchicalFunction, Unknown};

public:
    AbstractFunction();

    void setName(const QString &name);
    QString getName() const;
    QString getVariable() const;
    void setType(AbstractFunction::FunctionType type);
    AbstractFunction::FunctionType getType() const;

    /**
     * Virtual functions. Every function must have these to be a valid function
     **/
    virtual double compute(double) = 0;
    virtual double operator() (double x) = 0;
    virtual QString getExpression() const = 0;
    virtual void save(const QString &group);

protected:
    void abstractsave(const QString &group);

protected:
    QString mName;
    QString mVariable;
    FunctionType mType;

};

Q_DECLARE_METATYPE(AbstractFunction*);

#endif // ABSTRACTFUNCTION_H
