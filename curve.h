#ifndef CURVE_H
#define CURVE_H

#include <qwt_plot_curve.h>

class Curve: public QwtPlotCurve
{
public:
    Curve( const QString &name );

    void initData();
    void setResolution(int resolution);

private:
    void init();


private:
    QString mName;

    int mResolution;
};


#endif // CURVE_H
