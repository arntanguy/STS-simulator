#ifndef CURVE_H
#define CURVE_H

#include <qwt_plot_curve.h>
#include "data.h"


class Curve: public QObject, public QwtPlotCurve
{
    Q_OBJECT
public:
    Curve();
    Curve(int id);
    Curve( const QString &name );

    void setResolution(int resolution);
    int getResolution() const {
        return mResolution;
    }

    unsigned int getId() const {
        return mCurveId;
    }

    Data::Type getType() const {
        return mData->getType();
    }

    void save();
    void loadFromSettings();

    void setExperimentalData(const QString &experimentId, const QString &abscissiaColumnName, const QString &ordinateColumnName);

    virtual void update();
    bool needsUpdate() const {
        return mNeedsUpdate;
    }

private:
    void init();
    void setId(unsigned int id);


private:
    Data *mData;
    QString mExperimentalAbscissia, mExperimentalOrdinate;

    int mResolution;
    static unsigned int mCurveStaticId;
    unsigned int mCurveId;

protected:
    bool mNeedsUpdate;
};

Q_DECLARE_METATYPE(Curve*)


#endif // CURVE_H
