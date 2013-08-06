#include "differentialcurve.h"
#include "differentialfunction.h"
#include "plotwidget.h"

DifferentialCurve::DifferentialCurve() : FunctionCurve()
{
}

DifferentialCurve::DifferentialCurve(int id) : FunctionCurve(id)
{
}

void DifferentialCurve::init()
{
    setType(Curve::Differential);
}

void DifferentialCurve::update(bool forceUpdate)
{
    qDebug() << "DifferentialCurve::update()";
    FunctionCurve::update(forceUpdate);
}


void DifferentialCurve::attach(PlotWidget *plot)
{
    FunctionCurve::attach(plot);
    qDebug() << "PLOT: "<<plot->getId();
    //qDebug() << "DifferentialCurve::attach() - attach curve " << title().text() << " to plot " << plot->getName();
    //Curve *curve = 0;
    //if(isAttached(plot)) {
    //    qDebug() << "Curve already attached, update copies";
    //    curve = mPlots[plot];
    //    if(curve != 0) {
    //        curve->copyFromCurve(this);
    //    }
    //} else {
    //    qDebug() << "DifferentialCurve::attach() - create new curve";
    //    DifferentialCurve *c = new DifferentialCurve(getId());
    //    c->copyFromCurve(this);
    //    c->QwtPlotItem::attach(plot);
    //    curve = c;
    //}
    //qDebug() << "Attach curve====";
    //qDebug() << "curve "<< curve << " attached";
    //mPlots[plot] = curve;
    //qDebug() << "curve from plot "<< mPlots[plot] << " attached";
    //mPlots[plot] = this;
}

void DifferentialCurve::copyFromCurve(Curve *curve)
{
    qDebug() <<"ABCG";
    qDebug() << "DifferentialCurve::copyFromCurve()";
    Curve::abstractCopyFromCurve(curve);
    DifferentialCurve *fc = dynamic_cast<DifferentialCurve*>(curve);
    if(fc != 0) {
        setResolution(fc->getResolution());
        DifferentialFunction *f = dynamic_cast<DifferentialFunction*>(fc->getFunction());
        if(f != 0) {
            setFunction(f);
        }
    } else {
        qDebug() << "======°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°";
        qDebug() << "Copy from null!";
    }
}

void DifferentialCurve::save()
{
    FunctionCurve::save();
}

void DifferentialCurve::loadFromSettings()
{
    FunctionCurve::loadFromSettings();
}
/**
 * Differentiate
 **/

void DifferentialCurve::updateData()
{
    qDebug() << "=============++++++++++++++++=================";
    qDebug() << "DifferentialCurve::updateData()";
    qDebug() << "=============++++++++++++++++=================";
    if(mFunction != 0) {
        DifferentialFunction *f = dynamic_cast<DifferentialFunction *>(mFunction);
        if(f != 0) {
            PlotData data = f->differentiate();
           // qDebug() << "differential data";
           // qDebug() << "V: " <<data.x;
           // qDebug() << "f'(V): " <<data.y;
            mXData = data.x;
            mYData = data.y;
        } else {
            qDebug() << "DifferentialCurve::ERROR - Unable to compute differential: invalid function type.";
        }
    }
    mNeedsUpdate = false;
}


// ==================================== PUBLIC ==============================
void DifferentialCurve::setFunction(DifferentialFunction *f)
{
    qDebug() << "DifferentialCurve::setFunction";
    //XXX: handle this properly
    if(f != 0) {

        FunctionCurve::setFunction(f);
        qDebug() << "connecting function";
        connect(f, SIGNAL(integralDataComputed()), this, SLOT(slotUpdateData()));
        mFunction = f;
    } else {
        qDebug() << "DifferentialFunction::setFunction -- ERROR invalid function";
    }
}

// =================================== SLOTS ================================
void DifferentialCurve::slotUpdateData()
{
    qDebug() << "DifferentialCurve::slotUpdateData";
    update(true);
}
