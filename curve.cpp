#include "curve.h"
#include <algorithm>
#include "projectsingleton.h"

unsigned int Curve::mCurveStaticId = 0;

Curve::Curve()
{
    init();
    mResolution = 1;
    //XXX: temporary
    initData();
    setId(mCurveStaticId);
}

Curve::Curve(int id)
{
    setId(id);
}

Curve::Curve(const QString &name)
{
    init();
    mResolution = 1;
    //XXX: temporary
    initData();
    setTitle(name);
    setId(mCurveStaticId);
}

void Curve::init()
{
    setRenderHint( QwtPlotItem::RenderAntialiased );
}

void Curve::setId(unsigned int id)
{
    qDebug() << "set id " << id;
    if(id >= mCurveStaticId) {
        mCurveId = id;
        mCurveStaticId = std::max(id, mCurveStaticId) + 1;
    } else {
        // XXX: handle error
        mCurveId = -1;
    }
}

void Curve::initData()
{
//    QVector<QPointF> points;
//
//    double y = qrand() % 1000;
//
//    for ( double x = 0.0; x <= 1000.0; x += 1000.0/mResolution )
//    {
//        double off = qrand() % 200 - 100;
//        if ( y + off > 980.0 || y + off < 20.0 )
//            off = -off;
//
//        y += off;
//
//        points += QPointF( x, y );
//    }
//
//    setSamples( points );
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

/*!
 * \brief Curve::loadFromSettings
 *  Load curve from settings.
 *  WARNING: the curve id must be set beforehand Curve *curve = new Curve(42);
 *  Project settings state must be at root group
 */
void Curve::loadFromSettings()
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Curves/"+QString::number(mCurveId));
    setTitle(settings->value("title", "Unknown").toString());
    setResolution(settings->value("resolution", 0).toInt());

    // Load pen
    QPen pen;
    pen.setColor(settings->value("color", Qt::black).value<QColor>());
    pen.setWidth(settings->value("thickness", 0.0d).toDouble());
    pen.setStyle(static_cast<Qt::PenStyle>(settings->value("style", Qt::SolidLine).toInt()));
    // Apply pen
    setPen(pen);

    settings->endGroup();
}

void Curve::save()
{
    qDebug() << "Saving curve "<<mCurveId<<","<<title().text();
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Curves/"+QString::number(mCurveId)+"/");
    qDebug() << "current group: " << settings->group();
    settings->setValue("id", mCurveId);
    settings->setValue("title", title().text());
    settings->setValue("resolution", mResolution);

    settings->setValue("color", pen().color());
    settings->setValue("thickness", pen().width());
    settings->setValue("style", pen().style());
    settings->endGroup();
}
