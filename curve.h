#ifndef CURVE_H
#define CURVE_H

#include <qwt_plot_curve.h>

class Curve: public QwtPlotCurve
{
public:
    Curve( int index ):
        d_index( index )
    {
        setRenderHint( QwtPlotItem::RenderAntialiased );
        initData();
    }

    void setCurveTitle( const QString &title )
    {
        QString txt("%1 %2");
        setTitle( QString( "%1 %2" ).arg( title ).arg( d_index ) );
    }

    void initData()
    {
        QVector<QPointF> points;

        double y = qrand() % 1000;

        for ( double x = 0.0; x <= 1000.0; x += 100.0 )
        {
            double off = qrand() % 200 - 100;
            if ( y + off > 980.0 || y + off < 20.0 )
                off = -off;

            y += off;

            points += QPointF( x, y );
        }

        setSamples( points );
    }

private:
    const int d_index;
};


#endif // CURVE_H
