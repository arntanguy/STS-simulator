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

    void addCurve(PlotWidget *plot, FunctionCurve *curve);
    FunctionCurve* getCurve(PlotWidget *plot);

protected:
    void abstractsave(const QString &group);

Q_SIGNALS:
    void nameUpdated(const QString &);

public Q_SLOTS:
    void updateLinkedCurve(QString var, double val);

protected:
    QString mName;
    QString mVariable;
    FunctionType mType;

    bool mNeedsUpdate;

    QMap<PlotWidget *, FunctionCurve *> mLinkedCurves;

};

Q_DECLARE_METATYPE(AbstractFunction*);

#endif // ABSTRACTFUNCTION_H
