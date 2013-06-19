#ifndef CURVE_H
#define CURVE_H

#include <qwt_plot_curve.h>

class Data;

class Curve: public QwtPlotCurve
{
public:
    Curve();
    Curve(int id);
    Curve( const QString &name );

    void setResolution(int resolution);

    unsigned int getId() const {
        return mCurveId;
    }

    void save();
    void loadFromSettings();

    void setExperimentalData(const QString &experimentId, const QString &abscissiaColumnName, const QString &ordinateColumnName);

private:
    void init();
    void setId(unsigned int id);


private:
    Data *mData;
    QString mExperimentalAbscissia, mExperimentalOrdinate;

    int mResolution;
    static unsigned int mCurveStaticId;
    unsigned int mCurveId;
};

Q_DECLARE_METATYPE(Curve*)


#endif // CURVE_H
