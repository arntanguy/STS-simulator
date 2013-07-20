#ifndef CURVE_H
#define CURVE_H

#include <qwt_plot_curve.h>
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
    enum Type { Experimental, Function };
    Curve();
    Curve(int id);
    Curve( const QString &name );

    void copyFromCurve(Curve *curve);

    void setResolution(int resolution);
    int getResolution() const {
        return mResolution;
    }

    unsigned int getId() const {
        return mCurveId;
    }

    Data *getData() {
        return mData;
    }

    Type getType() const {
        return mType;
    }

    virtual void save();
    void loadFromSettings();

    void setExperimentalData(const QString &experimentId, const QString &abscissiaColumnName, const QString &ordinateColumnName);

    virtual void update();
    bool needsUpdate() const {
        return mNeedsUpdate;
    }

    void setTitle(const QString& title);
    void setPen(const QPen& pen);

    void attach(PlotWidget *plot);
    void detach(PlotWidget *plot);
    bool isAttached(PlotWidget *plot) const;

private:
    void init();
    void setId(unsigned int id);


private:
    Data *mData;
    QString mExperimentalAbscissia, mExperimentalOrdinate;
    QString mExperimentalId;

    int mResolution;
    static unsigned int mCurveStaticId;
    unsigned int mCurveId;

protected:
    bool mNeedsUpdate;
    QMap<PlotWidget *, Curve *> mPlots;
    Type mType;
};

Q_DECLARE_METATYPE(Curve*)


#endif // CURVE_H
