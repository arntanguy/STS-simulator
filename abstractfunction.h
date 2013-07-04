#ifndef ABSTRACTFUNCTION_H
#define ABSTRACTFUNCTION_H

#include <QString>

class AbstractFunction
{
public:
    AbstractFunction();

    void setName(const QString &name);
    QString getName() const;
    QString getVariable() const;

    /**
     * Virtual functions. Every function must have these to be a valid function
     **/
    virtual double compute(double) = 0;
    virtual double operator() (double x) = 0;
    virtual QString getExpression() const = 0;

protected:
    QString mName;
    QString mVariable;
};

#endif // ABSTRACTFUNCTION_H
