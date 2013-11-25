#ifndef INTEGRALFUNCTION_H
#define INTEGRALFUNCTION_H

#include "hierarchicalfunction.h"
#include "plotdata.h"

#include <QVector>
#include <QMap>


class IntegralFunction : public HierarchicalFunction
{
    Q_OBJECT
public:
    enum Range { ZeroToV, MinusVToZero };

public:
    IntegralFunction();
    IntegralFunction(int id);
    virtual ~IntegralFunction();


    virtual void save(const QString &group);
    virtual void loadFromConfig(const QString &group);

    void setIntegrationVariable(const QString &variable);
    QString getIntegrationVariable() const;

    void setStepNumber(const int stepNumber);
    int getStepNumber() const;

    void setRange(Range range);
    Range getRange() const;

    QString getExpression() const;
    QString getExpressionParameters() const;
    QString getIntegralExpression() const;

    bool hasFunction(int id) const;

    virtual double compute(double);
    virtual double compute(const QString&, double);
    virtual PlotData integrate(double min, double max, double resolution, double stepNumber);
    virtual FunctionCurve* createCurve();

    void setParameters(const QSharedPointer<Function> &f, const QString &parameter);
    QString getParameters(const FunctionPtr &f) const;

    PlotData getData() {
        return mData;
    }

    void setTransmissionDZ(const QString &dzName);
    QString getTransmissionDZName() const {
        return mTransmissionDZ;
    }
Q_SIGNALS:
    void integralDataComputed();

private:
    void init();
    void setFunctionsIntegrationVariable();
    double integrateZeroToV(double h, double V);
    double integrateMinusVTo0(double h, double V);

private:
    double mStepNumber;
    Range mRange;

    QString mIntegrationVariable;
    QString mTransmissionDZ;

    //FunctionId, Parameter
    QMap<int, QString> mParameters;

    PlotData mData;

};

#endif // INTEGRALFUNCTION_H
