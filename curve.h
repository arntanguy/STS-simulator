#ifndef CURVE_H
#define CURVE_H

#include <qwt_plot_curve.h>
#include <QSharedPointer>
#include "data.h"

class PlotWidget;

/**
 * Careful when copying curve, data isn't copied for performance reasons it is shared
 * If deep copy of data is needed, implement the deepCopy() function
 **/
class Curve: public QObject, public QwtPlotCurve
{
    Q_OBJECT
public:
    enum Type { Experimental, Function, Integral, Differential, Unknown };
    Curve();
    Curve(int id);
    Curve( const QString &name );
    virtual ~Curve();

    virtual void copyFromCurve(Curve *curve);

    void setMinMax(double min, double max);
    double getMin() const {
        return mMin;
    }
    double getMax() const {
        return mMax;
    }

    unsigned int getId() const {
        return mCurveId;
    }

    /**
     * DO NOT DELETE THE RETURNED POINTER!!
     **/
    Data *getData() {
        return mData.data();
    }

    void setType(Type t) {
        mType = t;
    }
    Type getType() const {
        return mType;
    }

    virtual void save();
    void loadFromSettings();

    void setExperimentalData(const QString &experimentId, const QString &abscissiaColumnName, const QString &ordinateColumnName);
    void updateExperimentalData();

    virtual void update(bool forceUpdate = false);
    void setUpdateNeeded() {
        qDebug() <<"========== force update=============";
        mNeedsUpdate = true;
    }
    bool needsUpdate() const {
        return mNeedsUpdate;
    }

    void setTitle(const QString& title);
    void setPen(const QPen& pen);

    virtual void attach(PlotWidget *plot);
    void detach(PlotWidget *plot);
    void detachFromAll();
    bool isAttached(PlotWidget *plot) const;
    bool isAttached(unsigned int plotId) const;

    // XXX: should be hidden (friend?)
    static void resetStaticId();
private:
    void init();
    void setId(int id);

protected:
    void abstractCopyFromCurve(Curve *curve);

Q_SIGNALS:
    virtual void rangeUpdated();

private:
    QSharedPointer<Data> mData;
    QString mExperimentalAbscissia, mExperimentalOrdinate;
    QString mExperimentalId;

    double mMin, mMax;

    static int mCurveStaticId;
    int mCurveId;

protected:
    bool mNeedsUpdate;
    QMap<PlotWidget *, Curve *> mPlots;
    Type mType;
};

Q_DECLARE_METATYPE(Curve*)


#endif // CURVE_H
