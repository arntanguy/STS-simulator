#include "curve.h"

Curve::Curve(const QString &name) : mName(name)
{
    init();
    mResolution = 1;
    //XXX: temporary
    initData();
}

void Curve::init()
{
    setTitle(mName);
    setRenderHint( QwtPlotItem::RenderAntialiased );
}

void Curve::initData()
{
    QVector<QPointF> points;

    double y = qrand() % 1000;

    for ( double x = 0.0; x <= 1000.0; x += 1000.0/mResolution )
    {
        double off = qrand() % 200 - 100;
        if ( y + off > 980.0 || y + off < 20.0 )
            off = -off;

        y += off;

        points += QPointF( x, y );
    }

    setSamples( points );
}

/*!
 * \brief Curve::setResolution
 *  Sets the number of points to be computed for the curve
 *  Careful, changing this value could result in all the points being recomputed,
 *  which could be quite an expensive operation!
 * \param resolution
 */
void Curve::setResolution(int resolution)
{
    mResolution = resolution;
    initData();
}
