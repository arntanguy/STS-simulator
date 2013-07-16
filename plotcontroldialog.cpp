#include <QSettings>
#include <QDebug>
#include "plotcontroldialog.h"
#include "ui_plotcontroldialog.h"
#include "projectsingleton.h"
#include "functionssingleton.h"
#include "hierarchicalfunction.h"
#include "function.h"
#include "helperfunctions.h"
#include "curvesingleton.h"
#include "newcurvedialog.h"
#include "newfunctiondialog.h"
#include "hierarchicalfunctiondialog.h"
#include "curve.h"
#include "functioncurve.h"
#include "plotarea.h"
#include "plotwidget.h"
#include <qwt_plot.h>
#include <QStandardItem>
#include <QStandardItemModel>

PlotControlDialog::PlotControlDialog(const QString &plotName, PlotArea *parent) :
    QDialog(parent),
    ui(new Ui::PlotControlDialog)
{
    ui->setupUi(this);
    mPlot = parent->getPlotWidget();
    mPlotName = plotName;

    init();
    initFromConfig();

    // Dialog
    connect(this->ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(this->ui->buttonBox, SIGNAL(accepted()), this, SLOT(close()));
    connect(this->ui->buttonBox, SIGNAL(accepted()), parent, SLOT(plotConfigChanged()));
    connect(this->ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    // Plot page
    connect(this->ui->autoAbscissa, SIGNAL(toggled(bool)), this, SLOT(autoAbscissaChecked(bool)));
    connect(this->ui->autoOrdinate, SIGNAL(toggled(bool)), this, SLOT(autoOrdinateChecked(bool)));

    // Curve page
    connect(ui->newCurveButton, SIGNAL(clicked()), this, SLOT(newCurve()));
    connect(ui->curveSelection, SIGNAL(doubleClicked ( const QModelIndex &)), this, SLOT(editCurve(const QModelIndex &)));

    // Function page
    connect(ui->functionNew, SIGNAL(clicked()), this, SLOT(newFunction()));
    connect(ui->functionHierarchicalNew, SIGNAL(clicked()), this, SLOT(newHierarachicalFunction()));
    connect(ui->functionView, SIGNAL(doubleClicked ( const QModelIndex &)), this, SLOT(editFunction(const QModelIndex &)));
}

PlotControlDialog::~PlotControlDialog()
{
    delete ui;
}

// ================================================================================
// ================================= PRIVATE ======================================
// ================================================================================
/*!
 * \brief PlotControlDialog::init
 *  Init config dialog (combobox...)
 */
void PlotControlDialog::init()
{
    // Init Axis scale controls
    ui->horizontalAxisScale->addItem(tr("Linear"), "linear");
    ui->horizontalAxisScale->addItem(tr("Log10"), "log10");
    ui->verticalAxisScale->addItem(tr("Linear"), "linear");
    ui->verticalAxisScale->addItem(tr("Log10"), "log10");

    // Init legend position controls
    ui->legendPositionComboBox->addItem(tr("Top"), QwtPlot::TopLegend);
    ui->legendPositionComboBox->addItem(tr("Bottom"), QwtPlot::BottomLegend);
    ui->legendPositionComboBox->addItem(tr("Left"), QwtPlot::LeftLegend);
    ui->legendPositionComboBox->addItem(tr("Right"), QwtPlot::RightLegend);

    ui->legendItemHorizontalPositionComboBox->addItem(tr("Left"), Qt::AlignLeft);
    ui->legendItemHorizontalPositionComboBox->addItem(tr("Center"), Qt::AlignHCenter);
    ui->legendItemHorizontalPositionComboBox->addItem(tr("Right"), Qt::AlignRight);

    ui->legendItemVerticalPositionComboBox->addItem(tr("Top"), Qt::AlignTop);
    ui->legendItemVerticalPositionComboBox->addItem(tr("Center"), Qt::AlignVCenter);
    ui->legendItemVerticalPositionComboBox->addItem(tr("Bottom"), Qt::AlignBottom);

    ui->majorGridPenStyle->addItem(tr("Solid Line"), Qt::SolidLine);
    ui->majorGridPenStyle->addItem(tr("Dot Line"), Qt::DotLine);
    ui->majorGridPenStyle->addItem(tr("Dash Line"), Qt::DashLine);
    ui->majorGridPenStyle->addItem(tr("Dash and Dot Line"), Qt::DashDotLine);
    ui->majorGridPenStyle->addItem(tr("Dash Dot Dot Dash Line"), Qt::DashDotDotLine);
    ui->minorGridPenStyle->addItem(tr("Solid Line"), Qt::SolidLine);
    ui->minorGridPenStyle->addItem(tr("Dot Line"), Qt::DotLine);
    ui->minorGridPenStyle->addItem(tr("Dash Line"), Qt::DashLine);
    ui->minorGridPenStyle->addItem(tr("Dash and Dot Line"), Qt::DashDotLine);
    ui->minorGridPenStyle->addItem(tr("Dash Dot Dot Dash Line"), Qt::DashDotDotLine);

    ui->curveSelection->setModel( new QStandardItemModel());
    ui->functionView->setModel( new QStandardItemModel());
    ui->functionView->setHeaderHidden(true);

    // TODO: load from config
    newCurveAvailable();
    newFunctionAvailable();
}

/*!
 * \brief PlotControlDialog::initFromConfig
 *  Initialize from saved configuration (QSettings)
 */
void PlotControlDialog::initFromConfig()
{
    QSettings *mSettings = Singleton<ProjectSingleton>::Instance().getSettings();

    qDebug() << "PlotControlDialog::initFromConfig()";
    mSettings->beginGroup("Plot/"+mPlotName+"/legend");
    ui->legendCheckBox->setChecked(mSettings->value("isEnabled", false).toBool());
    int index = ui->legendPositionComboBox->findData(mSettings->value("position", Qt::AlignBottom).toInt());
    if(index != -1)
        ui->legendPositionComboBox->setCurrentIndex(index);
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+mPlotName+"/legendItem");
    ui->legendItemCheckBox->setChecked(mSettings->value("isEnabled", true).toBool());
    QVariant align = static_cast<int>(Qt::AlignRight);
    index = ui->legendItemHorizontalPositionComboBox->findData(mSettings->value("horizontalPosition", align).toInt());
    if(index != -1)
        ui->legendItemHorizontalPositionComboBox->setCurrentIndex(index);
    align = static_cast<int>(Qt::AlignTop);
    index = ui->legendItemVerticalPositionComboBox->findData(mSettings->value("verticalPosition", align).toInt());
    if(index != -1)
        ui->legendItemVerticalPositionComboBox->setCurrentIndex(index);
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+mPlotName+"/info");
    ui->titleLineEdit->setText(mSettings->value("title", mPlotName).toString());
    ui->horizontalAxisLineEdit->setText(mSettings->value("horizontalAxisName", "X Axis").toString());
    ui->verticalAxisLineEdit->setText(mSettings->value("verticalAxisName", "X Axis").toString());
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+mPlotName+"/axisScale");
    index = ui->horizontalAxisScale->findData(mSettings->value("horizontalAxisScale", "linear").toString());
    if(index != -1)
        ui->horizontalAxisScale->setCurrentIndex(index);
    index = ui->verticalAxisScale->findData(mSettings->value("verticalAxisScale", "linear").toString());
    if(index != -1)
        ui->verticalAxisScale->setCurrentIndex(index);
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+mPlotName+"/precision");
    ui->plotResolution->setValue(mSettings->value("resolution", 1000).toDouble());
    mSettings->endGroup();

    QStringList enabledCurveIds = mSettings->value("Plot/"+mPlotName+"/curves/enabledCurveIds", QStringList()).toStringList();
    qDebug() << "loading enabled curve ids: "<<enabledCurveIds;
    Curve *curve = 0;
    foreach(QStandardItem *item, mCurveItems) {
        // Only show selected curves
        curve = static_cast<Curve *>(item->data(Qt::UserRole).value<Curve *>());
        if(curve != 0) {
            if( enabledCurveIds.contains(QString::number(curve->getId())) ) {
                qDebug() << "curve " << curve->getId() << " enabled";
                // TODO: save which curve is attached.
                item->setCheckState(Qt::Checked);
            } else {
                item->setCheckState(Qt::Unchecked);
            }
        } else {
            qDebug() << "null curve";
        }
    }

    // ========= Plot range ===========
    mSettings->beginGroup("Plot/"+mPlotName+"/range");
    autoAbscissaChecked(mSettings->value("autoAbscissa", true).toBool());
    ui->minAbscissaRange->setValue(mSettings->value("minAbscissa", 0).toDouble());
    ui->maxAbscissaRange->setValue(mSettings->value("maxAbscissa", 1000).toDouble());

    autoOrdinateChecked(mSettings->value("autoOrdinate", true).toBool());
    ui->minOrdinateRange->setValue(mSettings->value("minOrdinate", 0).toDouble());
    ui->maxOrdinateRange->setValue(mSettings->value("maxOrdinate", 1000).toDouble());
    mSettings->endGroup();

    // =============== Plot Grid ==================
    mSettings->beginGroup("Plot/"+mPlotName+"/grid");
    ui->gridEnabled->setChecked(mSettings->value("isEnabled", true).toBool());

    index = ui->majorGridPenStyle->findData(mSettings->value("majorPen/style", Qt::SolidLine).toString());
    if(index != -1)
        ui->majorGridPenStyle->setCurrentIndex(index);
    ui->majorGridAbscissiaPenEnabled->setChecked(mSettings->value("majorPen/abscissiaIsEnabled", true).toBool());
    ui->majorGridOrdinatePenEnabled->setChecked(mSettings->value("majorPen/ordinateIsEnabled", true).toBool());
    ui->majorGridPenWidth->setValue(mSettings->value("majorPen/width", 0.0).toDouble());

    index = ui->minorGridPenStyle->findData(mSettings->value("minorPen/style", Qt::DotLine).toString());
    if(index != -1)
        ui->minorGridPenStyle->setCurrentIndex(index);
    ui->minorGridAbscissiaPenEnabled->setChecked(mSettings->value("minorPen/abscissiaIsEnabled", true).toBool());
    ui->minorGridOrdinatePenEnabled->setChecked(mSettings->value("minorPen/ordinateIsEnabled", true).toBool());
    ui->minorGridPenWidth->setValue(mSettings->value("minorPen/width", 0.0).toDouble());
    mSettings->endGroup();

}

void PlotControlDialog::manageFunctionCurveFromItem(QStandardItem *item)
{
    AbstractFunction* function = static_cast<AbstractFunction *>(item->data(Qt::UserRole).value<AbstractFunction *>());
    if(function != 0) {
        if(item->checkState() == Qt::Checked) {
            // XXX: create curves properly
            FunctionCurve *c = function->getCurve(mPlot);
            if(c != 0) {
                qDebug() << "PlotControlDialog::manageFunctionCurveFromItem() - Function already has a curve, use it";
                c->update();
                c->attach(mPlot);
            } else {
                qDebug() << "PlotControlDialog::manageFunctionCurveFromItem() - Function doesn't have a curve, use it";
                FunctionCurve *fcurve = new FunctionCurve();
                fcurve->setFunction(function);
                fcurve->setComputeRange(ui->minAbscissaRange->value(), ui->maxAbscissaRange->value(), ui->plotResolution->value());
                // TODO: save which curve is attached.
                fcurve->attach(mPlot);
                fcurve->update();
                function->addCurve(mPlot, fcurve);
            }
        } else {
            FunctionCurve *c = function->getCurve(mPlot);
            if(c != 0) {
                if(c->plot() == mPlot)
                    c->detach();
            }
            // XXXX: IMPORTANT: curve can only be linked to one plot!!!
            //if(fcurve->plot() == mPlot) fcurve->detach();
        }
    } else {
        qDebug() << "PlotControlDialog::manageFunctionCurveFromItem() - NULL Curve";
    }
}


// ================================================================================
// ============================== PUBLIC ==========================================
// ================================================================================
/*!
 * \brief PlotControlDialog::accept
 *  Saves all the parameters set within the config dialog (QSettings)
 *  Discards config dialog
 */
void PlotControlDialog::accept()
{
    QSettings *mSettings = Singleton<ProjectSingleton>::Instance().getSettings();
    qDebug() << "PlotControlDialog::accept()";
    mSettings->beginGroup("Plot/"+mPlotName+"/legend");
    mSettings->setValue("isEnabled", ui->legendCheckBox->isChecked());
    mSettings->setValue("position", ui->legendPositionComboBox->itemData(ui->legendPositionComboBox->currentIndex()));
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+mPlotName+"/legendItem");
    mSettings->setValue("isEnabled", ui->legendItemCheckBox->isChecked());
    int horizontalPos = ui->legendItemHorizontalPositionComboBox->itemData(ui->legendItemHorizontalPositionComboBox->currentIndex()).toInt();
    int verticalPos = ui->legendItemVerticalPositionComboBox->itemData(ui->legendItemVerticalPositionComboBox->currentIndex()).toInt();
    Qt::Alignment aX = static_cast<Qt::Alignment>(horizontalPos);
    Qt::Alignment aY = static_cast<Qt::Alignment>(verticalPos);
    mSettings->setValue("horizontalPosition", horizontalPos);
    mSettings->setValue("verticalPosition", verticalPos);
    mSettings->setValue("alignment", static_cast<int>(aX|aY));
    mSettings->setValue("numCurves", 3);
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+mPlotName+"/range");
    mSettings->setValue("autoAbscissa", ui->autoAbscissa->isChecked());
    mSettings->setValue("minAbscissa", ui->minAbscissaRange->value());
    mSettings->setValue("maxAbscissa", ui->maxAbscissaRange->value());
    mSettings->setValue("autoOrdinate", ui->autoOrdinate->isChecked());
    mSettings->setValue("minOrdinate", ui->minOrdinateRange->value());
    mSettings->setValue("maxOrdinate", ui->maxOrdinateRange->value());
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+mPlotName+"/info");
    mSettings->setValue("title", ui->titleLineEdit->text());
    mSettings->setValue("horizontalAxisName", ui->horizontalAxisLineEdit->text());
    mSettings->setValue("verticalAxisName", ui->verticalAxisLineEdit->text());
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+mPlotName+"/axisScale");
    mSettings->setValue("horizontalAxisScale", ui->horizontalAxisScale->itemData(ui->horizontalAxisScale->currentIndex()));
    mSettings->setValue("verticalAxisScale", ui->verticalAxisScale->itemData(ui->verticalAxisScale->currentIndex()));
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+mPlotName+"/precision");
    mSettings->setValue("resolution", ui->plotResolution->value());
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+mPlotName+"/grid");
    mSettings->setValue("isEnabled", ui->gridEnabled->isChecked());
    mSettings->setValue("majorPen/abscissiaIsEnabled", ui->majorGridAbscissiaPenEnabled->isChecked());
    mSettings->setValue("majorPen/ordinateIsEnabled", ui->majorGridOrdinatePenEnabled->isChecked());
    mSettings->setValue("majorPen/style", ui->majorGridPenStyle->itemData(ui->majorGridPenStyle->currentIndex()));
    mSettings->setValue("majorPen/width", ui->majorGridPenWidth->value());

    mSettings->setValue("minorPen/abscissiaIsEnabled", ui->minorGridAbscissiaPenEnabled->isChecked());
    mSettings->setValue("minorPen/ordinateIsEnabled", ui->minorGridOrdinatePenEnabled->isChecked());
    mSettings->setValue("minorPen/style", ui->minorGridPenStyle->itemData(ui->minorGridPenStyle->currentIndex()));
    mSettings->setValue("minorPen/width", ui->minorGridPenWidth->value());
    mSettings->endGroup();

    // Display selected curves
    QStringList enabledCurveIds;
    Curve *curve = 0;
    foreach(QStandardItem *item, mCurveItems) {
        // Only show selected curves
        curve = static_cast<Curve *>(item->data(Qt::UserRole).value<Curve *>());
        if(curve != 0) {
            if(item->checkState() == Qt::Checked) {
                // TODO: save which curve is attached.
                curve->attach(mPlot);
                enabledCurveIds << QString::number(curve->getId());
            } else {
                // XXXX: IMPORTANT: curve can only be linked to one plot!!!
                if(curve->plot() == mPlot) curve->detach();
            }
        } else {
            qDebug() << "NULL curve";
        }
    }
    qDebug() << "enabled curves " << enabledCurveIds;
    mSettings->beginGroup("Plot/"+mPlotName+"/curves");
    mSettings->setValue("enabledCurveIds", enabledCurveIds);
    mSettings->endGroup();

    AbstractFunction *function = 0;
    foreach(QStandardItem *item, mFunctionItems) {
        manageFunctionCurveFromItem(item);

        // Handle item children (subfunctions)
        QStandardItem *child = 0;
        int i = 0;
        while ((child = item->child(i++)) != 0) {
            manageFunctionCurveFromItem(child);
        }
    }

    mSettings->sync();
    QDialog::accept();
}


// =========================== SLOTS =================================
void PlotControlDialog::autoAbscissaChecked(bool checked)
{
    ui->autoAbscissa->setChecked(checked);
    ui->minAbscissaRange->setEnabled(!checked);
    ui->maxAbscissaRange->setEnabled(!checked);
}

void PlotControlDialog::autoOrdinateChecked(bool checked)
{
    ui->autoOrdinate->setChecked(checked);
    ui->minOrdinateRange->setEnabled(!checked);
    ui->maxOrdinateRange->setEnabled(!checked);
}

void PlotControlDialog::newCurve()
{
    NewCurveDialog dialog(this);
    connect(&dialog, SIGNAL(accepted()), this, SLOT(newCurveAvailable()));
    dialog.exec();
}

void PlotControlDialog::editCurve(const QModelIndex &index)
{
    NewCurveDialog dialog(this);
    dialog.setWindowTitle(tr("Edit Curve"));
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->curveSelection->model());
    if(model != 0) {
        QVariant item = model->data(index, Qt::UserRole);
        if(item.isValid()) {
            qDebug() << item;
            Curve *curve = static_cast<Curve *>(item.value<Curve *>());
            if(curve != 0) {
                dialog.loadFromCurve(curve);
            }
        }
    }
    connect(&dialog, SIGNAL(accepted()), this, SLOT(newCurveAvailable()));
    dialog.exec();
}

void PlotControlDialog::newCurveAvailable()
{
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->curveSelection->model());
    model->clear();
    mCurveItems.clear();

    QMap<unsigned int, Curve *> map = Singleton<CurveSingleton>::Instance().getCurves();
    QMapIterator<unsigned int, Curve*> i(map);
    int j=0;
    while (i.hasNext()) {
        i.next();
        QStandardItem *Item = new QStandardItem();
        Item->setCheckable( true );
        // XXX: doesn't work, check if attached to plot
        if(i.value()->plot() == mPlot)
            Item->setCheckState( Qt::Checked );
        else
            Item->setCheckState( Qt::Unchecked );
        Item->setEditable(false);
        Item->setText(i.value()->title().text());
        Item->setData(QVariant::fromValue(i.value()), Qt::UserRole);
        mCurveItems.append(Item);
        model->setItem( j++, Item );
    }
}

void PlotControlDialog::newFunction()
{
    NewFunctionDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted)	{
        newFunctionAvailable();
    }
}

void PlotControlDialog::newHierarachicalFunction()
{
    HierarchicalFunctionDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted)	{
        newFunctionAvailable();
    }
}

void PlotControlDialog::newFunctionAvailable()
{
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->functionView->model());
    model->clear();
    mFunctionItems.clear();

    int itemIndex=0;
    FunctionsSingleton *fSingleton = &Singleton<FunctionsSingleton>::Instance();
    QStringList fNames = fSingleton->getFunctionNames();
    foreach(QString fName, fNames) {
        AbstractFunction *f = fSingleton->getFunction(fName);
        HierarchicalFunction *hf = dynamic_cast<HierarchicalFunction*>(f);
        if(hf != 0) {
            qDebug() << "add hierachical function to view";
            QStandardItem *parentItem = HelperFunctions::createFunctionItem(f);
            model->setItem(itemIndex++, parentItem);
            mFunctionItems.append(parentItem);

            int subItemIndex = 0;
            foreach(AbstractFunction *af, hf->getFunctions()) {
                QStandardItem * item = HelperFunctions::createFunctionItem(af);
                parentItem->setChild(subItemIndex++,item);
            }
        } else {
            Function *ff = dynamic_cast<Function*>(f);
            if(ff != 0) {
                qDebug() << "Add normal function to view";
                QStandardItem *Item = HelperFunctions::createFunctionItem(f);
                mFunctionItems.append(Item);
                model->setItem( itemIndex++, Item );
            }
        }

    }
}

void PlotControlDialog::editFunction(const QModelIndex &index)
{
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->functionView->model());
    if(model != 0) {
        QVariant item = model->data(index, Qt::UserRole);
        if(item.isValid()) {
            qDebug() << item;
            AbstractFunction *f = static_cast<AbstractFunction *>(item.value<AbstractFunction *>());
            if(f != 0) {
                Function *func = dynamic_cast<Function *>(f);
                if(func != 0) {
                    NewFunctionDialog dialog(func, this);
                    if(dialog.exec() == QDialog::Accepted) {
                        newFunctionAvailable();
                    }
                } else {
                    HierarchicalFunction *function = dynamic_cast<HierarchicalFunction *>(f);
                    if(function != 0) {
                        HierarchicalFunctionDialog dialog(this);
                        dialog.setFunction(function);
                        if(dialog.exec() == QDialog::Accepted) {
                            newFunctionAvailable();
                        }
                    }
                }
                qDebug() << "Editing function " << f->getName();
            }
        }
    }
}
