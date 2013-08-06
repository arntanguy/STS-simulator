#ifndef ABSTRACTFUNCTION_H
#define ABSTRACTFUNCTION_H

#include <QString>
#include <QMetaType>
#include <QObject>
#include <QMap>

class PlotWidget;
class FunctionCurve;

class AbstractFunction : public QObject
{
    Q_OBJECT

public:
    enum FunctionType { Function, HierarchicalFunction, Integral, Differential, Unknown};

public:
    AbstractFunction(QObject *parent=0);
    AbstractFunction(int id, QObject *parent=0);

    void setName(const QString &name);
    QString getName() const;
    QString getVariable() const;
    void setType(AbstractFunction::FunctionType type);
    AbstractFunction::FunctionType getType() const;

    /**
     * Virtual functions. Every function must have these to be a valid function
     **/
    virtual double compute(const QString&, double) = 0;
    virtual double compute(double) = 0;
    virtual double operator() (double x);
    virtual QString getExpression() const = 0;
    virtual void save(const QString &group);
    virtual void loadFromConfig(const QString &group);
    virtual void setGroup(const QString &group) {
        mBaseGroup = group;
    }
    virtual QString getGroup() const;
    QString getFunctionId() const;

    virtual FunctionCurve* createCurve();
    void setCurve(FunctionCurve *curve);
    FunctionCurve* getCurve();

    unsigned int getCurveId() const;
    void updateLinkedCurve(bool forceUpdate = false);

    bool isDisplayed(unsigned int plotId) const;

    void setId(int id);
    int getId() const {
        return mFunctionId;
    }

Q_SIGNALS:
    void nameUpdated(const QString &);
    virtual void curveUpdated(AbstractFunction *);
    virtual void functionUpdated(AbstractFunction *);
    void needsRecompute();

public Q_SLOTS:
    void updateLinkedCurve(QString var, double val, bool forceUpdate = false);

protected:
    void abstractsave(const QString &);

private:
    static int mFunctionStaticId;
    int mFunctionId;

protected:
    QString mName;
    QString mVariable;
    QString mBaseGroup;
    FunctionType mType;

    bool mNeedsUpdate;

    FunctionCurve *mLinkedCurve;
};

Q_DECLARE_METATYPE(AbstractFunction*);

#endif // ABSTRACTFUNCTION_H
