#ifndef ABSTRACTFUNCTION_H
#define ABSTRACTFUNCTION_H

#include <QString>
#include <QMetaType>
#include <QObject>

class AbstractFunction : public QObject
{
    Q_OBJECT

public:
    enum FunctionType { Function, HierarchicalFunction, Unknown};

public:
    AbstractFunction(QObject *parent=0);

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

Q_SIGNALS:
    void nameUpdated(const QString &);

protected:
    QString mName;
    QString mVariable;
    FunctionType mType;

    bool mNeedsUpdate;

};

Q_DECLARE_METATYPE(AbstractFunction*);

#endif // ABSTRACTFUNCTION_H
