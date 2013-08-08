#include "functioncurve.h"
#include "abstractfunction.h"
#include "muParser.h"
#include "plotwidget.h"
#include "projectsingleton.h"
#include "globalsettingssingleton.h"

#include <QDebug>
#include <QVector>
#include <QObject>

FunctionCurve::FunctionCurve() : Curve()
{
    init();
}

FunctionCurve::FunctionCurve(int id) : Curve(id)
{
    init();
}

FunctionCurve::~FunctionCurve()
{
    qDebug() << "FunctionCurve::~FunctionCurve - Destroying curve: id="<<getId()<< ", name=" << title().text();
    // XXX destroy data if not reference by any other curves
    // delete mData;
}

void FunctionCurve::init()
{
    mFunction = 0;
    mType = Curve::Function;

    initGlobalSettings();

    GlobalSettingsSingleton *singleton = &Singleton<GlobalSettingsSingleton>::Instance();
    connect(singleton, SIGNAL(curveSettingsUpdated()), this, SLOT(initGlobalSettings()));
}

void FunctionCurve::initGlobalSettings()
{
    GlobalSettingsSingleton *singleton = &Singleton<GlobalSettingsSingleton>::Instance();
    setMinMax(singleton->getMin(), singleton->getMax());
    setResolution(singleton->getResolution());
}

void FunctionCurve::setFunction(AbstractFunction *f)
{
    if(f != mFunction) {
        mFunction = f;
        setTitle(f->getName());
        qDebug() << "FunctionCurve::setFunction for function "<<f->getName();
        connect(mFunction, SIGNAL(needsRecompute()), this, SLOT(slotUpdateData()));
        connect(mFunction, SIGNAL(nameUpdated(const QString &)), this, SLOT(updateName(const QString &)));
        connect(mFunction, SIGNAL(expressionChanged()), this, SLOT(slotDeferedUpdate()));
        mNeedsUpdate = true;
        update();
    }
}

void FunctionCurve::setComputeRange(double min, double max, int resolution)
{
    if(min != getMin() || max != getMax() || resolution != getResolution()) {
        setMinMax(getMin(), getMax());
        setResolution(resolution);
        mNeedsUpdate = true;
    }
}

/*!
 * \brief Curve::setResolution
 *  Sets the number of points to be computed for the curve
 *  Careful, changing this value could result in all the points being recomputed,
 *  which could be quite an expensive operation!
 * \param resolution
 */
void FunctionCurve::setResolution(int resolution)
{
    if(mResolution != resolution) {
        mNeedsUpdate = true;
        mResolution = resolution;
    }
    update();
}

int FunctionCurve::getResolution() const
{
    return mResolution;
}


/// =============== VIRTUAL =========================
void FunctionCurve::copyFromCurve(Curve *curve)
{
    qDebug() << "FunctionCurve::copyFromCurve";
    Curve::abstractCopyFromCurve(curve);
    FunctionCurve *fc = dynamic_cast<FunctionCurve*>(curve);
    if(fc != 0) {
        setResolution(fc->getResolution());
        setFunction(fc->getFunction());
    }
}

void FunctionCurve::update(bool forceUpdate)
{
    qDebug() << "FunctionCurve::update()";
    if(needsUpdate() || forceUpdate) {
        qDebug() << "FunctionCurve::update()";
        updateData();
    }
    foreach(PlotWidget *plot, mPlots.keys()) {
        qDebug() << "Updating on plot " << plot->getName();
        Curve *curve = mPlots[plot];
        if(curve != 0) {
            qDebug() << "Updating " << curve->title().text();
            curve->setSamples(mXData, mYData);
        } else {
            qDebug() << "NULL CURVE!!!!!!!!!!!!!" << curve;
        }
        plot->replot();
    }
}

// XXX: FIXME: Huge value make program crash (std::bad_alloc)
void FunctionCurve::updateData()
{
    if(mFunction != 0) {
        qDebug() << "FunctionCurve::updateData()";
        int resolution = getResolution();
        mXData.clear();
        mYData.clear();
        double stepSize = 0;
        if(resolution != 0) {
            stepSize = (getMax()-getMin())/resolution;
        } else {
            qDebug() << "FunctionCurve::updateData() - CRITICAL ERROR: Resolution is null, no data computed!";
            return;
        }
        qDebug() << "FunctionCurve::updateData() - updating data of " << mFunction->getName()
            << " with resolution: "<<resolution<<", stepsize: "<<stepSize<<", min: "<<getMin()<<", max: "<<getMax();
        double xval = getMin();
        for(int i=0; i<resolution; i++) {
            double yval = 0;
            try {
                yval = mFunction->compute(xval);
            } catch(mu::Parser::exception_type &e) {
                qDebug() << "FunctionCurve::updateData() - ERROR computing function " << mFunction->getName();
                qDebug() << "Message:  " << e.GetMsg().c_str() << "\n";
                qDebug() << "Formula:  " << e.GetExpr().c_str() << "\n";
                qDebug() << "Token:    " << e.GetToken().c_str() << "\n";
                qDebug() << "Position: " << e.GetPos() << "\n";
                qDebug() << "Errc:     " << e.GetCode() << "\n";
                break;
            }
            mXData.append(xval);
            mYData.append(yval);
            xval += stepSize;
        }
        //setSamples(x, y);
    } else {
        qDebug() << "FunctionCurve::updateData() - error, null function";
    }
    mNeedsUpdate = false;
}

void FunctionCurve::save()
{
    Curve::save();
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Curves/"+QString::number(getId())+"/");
    settings->setValue("resolution", mResolution);
    settings->endGroup();
}

void FunctionCurve::loadFromSettings()
{
    qDebug() << "FunctionCurve::loadFromSettings()";
    Curve::loadFromSettings();

    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Curves/"+QString::number(getId())+"/");
    setResolution(settings->value("resolution", 10).toInt());
    settings->endGroup();
    update();
}

/// =============== SLOTS ==========================
void FunctionCurve::slotUpdateData()
{
    qDebug() << "FunctionCurve::slotUpdateData()";
    updateData();
}

void FunctionCurve::updateName(const QString &name)
{
    setTitle(name);
}

void FunctionCurve::slotDeferedUpdate()
{
    mNeedsUpdate = true;
}
