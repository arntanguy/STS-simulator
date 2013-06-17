#ifndef CURVE_H
#define CURVE_H

#include <qwt_plot_curve.h>

class Curve: public QwtPlotCurve
{
public:
    Curve();
    Curve( const QString &name );

    void initData();
    void setResolution(int resolution);

private:
    void init();


private:
    int mResolution;
};

Q_DECLARE_METATYPE(Curve*)


#endif // CURVE_H
