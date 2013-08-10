#include <QSettings>
#include <QDebug>
#include "plotcontrolwindow.h"
#include "ui_plotcontrolwindow.h"

#include "projectsingleton.h"
#include "functionssingleton.h"
#include "curvesingleton.h"

#include "function.h"
#include "hierarchicalfunction.h"
#include "differentialfunction.h"
#include "integralfunction.h"

#include "hierarchicalfunctiondialog.h"
#include "integralfunctiondialog.h"
#include "differentialfunctiondialog.h"

#include "curve.h"
#include "functioncurve.h"
#include "integralcurve.h"
#include "differentialcurve.h"

#include "newcurvedialog.h"
#include "newfunctiondialog.h"
#include "plotarea.h"
#include "plotwidget.h"

#include "helperfunctions.h"

#include <qwt_plot.h>
#include <QStandardItem>
#include <QStandardItemModel>

PlotControlWindow::PlotControlWindow(const int plotId, PlotArea *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotControlWindow)
{
    ui->setupUi(this);
    mPlot = parent->getPlotWidget();
    mPlotId = plotId;

    init();
    initFromConfig();

    // Dialog
    connect(this->ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(this->ui->buttonBox, SIGNAL(accepted()), this, SLOT(close()));
    connect(this->ui->buttonBox, SIGNAL(accepted()), parent, SLOT(plotConfigChanged()));
    connect(this->ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    QPushButton* applyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
    connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(ui->autoOrdinate, SIGNAL(toggled(bool)), this, SLOT(autoOrdinateChecked(bool)));

    // Curve page
    connect(ui->newCurveButton, SIGNAL(clicked()), this, SLOT(newCurve()));
    connect(ui->curveSelection, SIGNAL(doubleClicked ( const QModelIndex &)), this, SLOT(editCurve(const QModelIndex &)));
    connect(ui->deleteCurveButton, SIGNAL(clicked()), this, SLOT(deleteSelectedCurve()));

    // Function page
    connect(ui->functionNew, SIGNAL(clicked()), this, SLOT(newFunction()));
    connect(ui->functionHierarchicalNew, SIGNAL(clicked()), this, SLOT(newHierarachicalFunction()));
    connect(ui->functionIntegralNew, SIGNAL(clicked()), this, SLOT(newIntegralFunction()));
    connect(ui->functionDifferentialNew, SIGNAL(clicked()), this, SLOT(newDifferentialFunction()));
    connect(ui->functionView, SIGNAL(doubleClicked ( const QModelIndex &)), this, SLOT(editFunction(const QModelIndex &)));
    connect(ui->functionDelete, SIGNAL(clicked()), this, SLOT(deleteFunction()));
    connect(ui->functionEditCurve, SIGNAL(clicked()), this, SLOT(editFunctionCurve()));

    setWindowModality(Qt::NonModal);
}

PlotControlWindow::~PlotControlWindow()
{
    delete ui;
}

// ================================================================================
// ================================= PRIVATE ======================================
// ================================================================================
/*!
 * \brief PlotControlWindow::init
 *  Init config dialog (combobox...)
 */
void PlotControlWindow::init()
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
 * \brief PlotControlWindow::initFromConfig
 *  Initialize from saved configuration (QSettings)
 */
void PlotControlWindow::initFromConfig()
{
    QSettings *mSettings = Singleton<ProjectSingleton>::Instance().getSettings();

    qDebug() << "PlotControlWindow::initFromConfig() - plot id " << mPlotId;
    mSettings->beginGroup("Plot/"+QString::number(mPlotId)+"/legend");
    ui->legendCheckBox->setChecked(mSettings->value("isEnabled", false).toBool());
    int index = ui->legendPositionComboBox->findData(mSettings->value("position", Qt::AlignBottom).toInt());
    if(index != -1)
        ui->legendPositionComboBox->setCurrentIndex(index);
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+QString::number(mPlotId)+"/legendItem");
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

    mSettings->beginGroup("Plot/"+QString::number(mPlotId)+"/info");
    ui->titleLineEdit->setText(mSettings->value("title", QString::number(mPlotId)).toString());
    ui->horizontalAxisLineEdit->setText(mSettings->value("horizontalAxisName", "X Axis").toString());
    ui->verticalAxisLineEdit->setText(mSettings->value("verticalAxisName", "X Axis").toString());
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+QString::number(mPlotId)+"/axisScale");
    index = ui->horizontalAxisScale->findData(mSettings->value("horizontalAxisScale", "linear").toString());
    if(index != -1)
        ui->horizontalAxisScale->setCurrentIndex(index);
    index = ui->verticalAxisScale->findData(mSettings->value("verticalAxisScale", "linear").toString());
    if(index != -1)
        ui->verticalAxisScale->setCurrentIndex(index);
    mSettings->endGroup();

    Curve *curve = 0;
    foreach(QStandardItem *item, mCurveItems) {
        // Only show selected curves
        curve = static_cast<Curve *>(item->data(Qt::UserRole).value<Curve *>());
        if(curve != 0 && curve->getType() == Curve::Experimental) {
            if( curve->isAttached(mPlotId) ) {
                qDebug() << "curve " << curve->getId() << " enabled";
                item->setCheckState(Qt::Checked);
            } else {
                item->setCheckState(Qt::Unchecked);
            }
        } else {
            qDebug() << "null curve";
        }
    }
    foreach(QStandardItem *item, mFunctionItems) {
        // Only show selected curves
        AbstractFunctionPtr function = static_cast<AbstractFunctionPtr>(item->data(Qt::UserRole).value<AbstractFunctionPtr>());
        if(function != 0) {
            FunctionCurve *c = function->getCurve();
            if(c != 0) {
                if( c->isAttached(mPlotId) ) {
                    qDebug() << "curve " << c->getId() << " enabled";
                    item->setCheckState(Qt::Checked);
                } else {
                    item->setCheckState(Qt::Unchecked);
                }
            }
        }

        // Handle item children (subfunctions)
        QStandardItem *child = 0;
        int i = 0;
        while ((child = item->child(i++)) != 0) {
            function = child->data(Qt::UserRole).value<AbstractFunctionPtr>();
            if(function != 0) {
                FunctionCurve *c = function->getCurve();
                if(c != 0) {
                    if( c->isAttached(mPlotId) ) {
                        qDebug() << "curve " << c->getId() << " enabled";
                        child->setCheckState(Qt::Checked);
                    } else {
                        child->setCheckState(Qt::Unchecked);
                    }
                }
            }
        }
    }

    // ========= Plot range ===========
    mSettings->beginGroup("Plot/"+QString::number(mPlotId)+"/range");
    autoOrdinateChecked(mSettings->value("autoOrdinate", true).toBool());
    ui->minOrdinateRange->setValue(mSettings->value("minOrdinate", 0).toDouble());
    ui->maxOrdinateRange->setValue(mSettings->value("maxOrdinate", 1000).toDouble());
    mSettings->endGroup();


    // =============== Plot Grid ==================
    mSettings->beginGroup("Plot/"+QString::number(mPlotId)+"/grid");
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

/**
 * Create/activate curve from function item
 * return: the curve if exists and active
 *         0 otherwise
 **/
FunctionCurve* PlotControlWindow::manageFunctionCurveFromItem(QStandardItem *item)
{
    AbstractFunctionPtr function = item->data(Qt::UserRole).value<AbstractFunctionPtr>();
    if(function != 0) {
        FunctionCurve *c = function->getCurve();
        if(item->isCheckable() && item->checkState() == Qt::Checked) {
            if(c != 0) {
                qDebug() << "PlotControlWindow::manageFunctionCurveFromItem() - Function already has a curve, use it";
                c->attach(mPlot);
                c->update();
                if(Singleton<CurveSingleton>::Instance().getCurve(c->getId()) == 0)
                    Singleton<CurveSingleton>::Instance().addCurve(c);
                return c;
            } else {
                qDebug() << "PlotControlWindow::manageFunctionCurveFromItem() - Function doesn't have a curve, create it";
                FunctionCurve *fcurve = function->createCurve();
                // TODO: save which curve is attached.
                function->setCurve(fcurve);
                fcurve->attach(mPlot);
                fcurve->update(true);
                Singleton<CurveSingleton>::Instance().addCurve(fcurve);
                return fcurve;
            }
        } else {
            if(item->isCheckable()) {
                if(c != 0) {
                    c->detach(mPlot);
                }
            }
            return 0;
        }
    } else {
        qDebug() << "PlotControlWindow::manageFunctionCurveFromItem() - NULL Curve";
    }
    return 0;
}

// =========================== SLOTS =================================
void PlotControlWindow::apply()
{
    QSettings *mSettings = Singleton<ProjectSingleton>::Instance().getSettings();
    qDebug() << "PlotControlWindow::accept()";
    mSettings->beginGroup("Plot/"+QString::number(mPlotId)+"/legend");
    mSettings->setValue("isEnabled", ui->legendCheckBox->isChecked());
    mSettings->setValue("position", ui->legendPositionComboBox->itemData(ui->legendPositionComboBox->currentIndex()));
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+QString::number(mPlotId)+"/legendItem");
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

    mSettings->beginGroup("Plot/"+QString::number(mPlotId)+"/range");
    mSettings->setValue("autoOrdinate", ui->autoOrdinate->isChecked());
    mSettings->setValue("minOrdinate", ui->minOrdinateRange->value());
    mSettings->setValue("maxOrdinate", ui->maxOrdinateRange->value());
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+QString::number(mPlotId)+"/info");
    mSettings->setValue("title", ui->titleLineEdit->text());
    mSettings->setValue("horizontalAxisName", ui->horizontalAxisLineEdit->text());
    mSettings->setValue("verticalAxisName", ui->verticalAxisLineEdit->text());
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+QString::number(mPlotId)+"/axisScale");
    mSettings->setValue("horizontalAxisScale", ui->horizontalAxisScale->itemData(ui->horizontalAxisScale->currentIndex()));
    mSettings->setValue("verticalAxisScale", ui->verticalAxisScale->itemData(ui->verticalAxisScale->currentIndex()));
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+QString::number(mPlotId)+"/grid");
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
    Curve *curve = 0;
    foreach(QStandardItem *item, mCurveItems) {
        // Only show selected curves
        curve = static_cast<Curve *>(item->data(Qt::UserRole).value<Curve *>());
        if(curve != 0) {
            if(item->checkState() == Qt::Checked) {
                curve->attach(mPlot);
            } else {
                curve->detach(mPlot);
            }
        } else {
            qDebug() << "NULL curve";
        }
    }

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
    mPlot->loadFromSettings();

}
/*!
 * \brief PlotControlWindow::accept
 *  Saves all the parameters set within the config dialog (QSettings)
 *  Discards config dialog
 */
void PlotControlWindow::accept()
{
    apply();
    emit accepted();
}

void PlotControlWindow::newCurve()
{
    NewCurveDialog dialog(this);
    connect(&dialog, SIGNAL(accepted()), this, SLOT(newCurveAvailable()));
    dialog.exec();
}

void PlotControlWindow::editCurve(const QModelIndex &index)
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

void PlotControlWindow::newCurveAvailable()
{
    qDebug() << "PlotControlWindow::newCurveAvailable()";
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->curveSelection->model());
    model->clear();
    mCurveItems.clear();

    QMap<int, Curve *> map = Singleton<CurveSingleton>::Instance().getCurves();
    QMapIterator<int, Curve*> i(map);
    int j=0;
    while (i.hasNext()) {
        i.next();
        if(i.value() != 0 && i.value()->getType() == Curve::Experimental) {
            QStandardItem *Item = new QStandardItem();
            Item->setCheckable( true );
            if(i.value()->isAttached(mPlot))
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
}

void PlotControlWindow::newFunction()
{
    NewFunctionDialog* dialog = new NewFunctionDialog(this);
    connect(dialog, SIGNAL(accepted()), this, SLOT(functionDialogAccepted()));
    dialog->show();
}

void PlotControlWindow::newHierarachicalFunction()
{
    HierarchicalFunctionDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted)	{
        newFunctionAvailable();
    }
}

void PlotControlWindow::newIntegralFunction()
{
    IntegralFunctionDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted)	{
        newFunctionAvailable();
    }
}

void PlotControlWindow::newDifferentialFunction()
{
    DifferentialFunctionDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted)	{
        newFunctionAvailable();
    }
}

void PlotControlWindow::newFunctionAvailable()
{
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->functionView->model());
    model->clear();
    mFunctionItems.clear();

    int itemIndex=0;
    FunctionsSingleton *fSingleton = &Singleton<FunctionsSingleton>::Instance();
    QList<int> fIds = fSingleton->getFunctionIds();
    foreach(int id, fIds) {
        AbstractFunctionPtr f = fSingleton->getFunctionById(id);
        if(f->getType() == AbstractFunction::HierarchicalFunction) {
            HierarchicalFunctionPtr hf = qSharedPointerDynamicCast<HierarchicalFunction>(f);
            if(hf != 0) {
               qDebug() << "add hierachical function to view";
               QStandardItem *parentItem = HelperFunctions::createFunctionItem(f);
               model->setItem(itemIndex++, parentItem);
               if(hf->isDisplayed(mPlotId)) parentItem->setCheckState(Qt::Checked);
               mFunctionItems.append(parentItem);

               int subItemIndex = 0;
               foreach(AbstractFunctionPtr af, hf->getFunctions()) {
                   QStandardItem * item = HelperFunctions::createFunctionItem(af);
                   if(af->isDisplayed(mPlotId)) item->setCheckState(Qt::Checked);
                   parentItem->setChild(subItemIndex++,item);
               }
            }
        } else if(f->getType() == AbstractFunction::Function) {
            FunctionPtr ff = qSharedPointerDynamicCast<Function>(f);
            if(ff != 0) {
                qDebug() << "Add normal function to view";
                QStandardItem *Item = HelperFunctions::createFunctionItem(f, false);
                //if(ff->isDisplayed(mPlotId)) Item->setCheckState(Qt::Checked);
                // XXX: disable display of base functions
                //Item->setCheckable(false);
                mFunctionItems.append(Item);
                model->setItem( itemIndex++, Item );
            }
        } else if(f->getType() == AbstractFunction::Integral) {
            IntegralFunctionPtr hf = qSharedPointerDynamicCast<IntegralFunction>(f);
            if(hf != 0) {
                qDebug() << "add integral function to view";
                QStandardItem *parentItem = HelperFunctions::createFunctionItem(f);
                model->setItem(itemIndex++, parentItem);
                if(hf->isDisplayed(mPlotId)) parentItem->setCheckState(Qt::Checked);
                mFunctionItems.append(parentItem);

                int subItemIndex = 0;
                foreach(AbstractFunctionPtr af, hf->getFunctions()) {
                    QStandardItem * item = HelperFunctions::createFunctionItem(af, false);
                    item->setCheckState(Qt::Unchecked);
                    item->setCheckable(false);
                    if(af->isDisplayed(mPlotId)) item->setCheckState(Qt::Checked);
                    parentItem->setChild(subItemIndex++,item);
                }
            }
        } else if(f->getType() == AbstractFunction::Differential) {
            DifferentialFunctionPtr ff = qSharedPointerDynamicCast<DifferentialFunction>(f);
            if(ff != 0) {
                qDebug() << "Add normal function to view";
                QStandardItem *Item = HelperFunctions::createFunctionItem(f);
                if(ff->isDisplayed(mPlotId)) Item->setCheckState(Qt::Checked);
                Item->setCheckable(true);
                mFunctionItems.append(Item);
                model->setItem( itemIndex++, Item );
            }
        }
    }
}

void PlotControlWindow::deleteFunction()
{
    qDebug() << "PlotControlWindow::deleteFunction()";
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->functionView->model());
    QModelIndex index = ui->functionView->currentIndex();
    AbstractFunctionPtr f = index.data(Qt::UserRole).value<AbstractFunctionPtr>();
    mFunctionItems.removeOne(model->itemFromIndex(index));
    Singleton<FunctionsSingleton>::Instance().removeFunction(f);
    model->removeRow(index.row());
    // XXX: More appropriate call?
    newFunctionAvailable();
}

void PlotControlWindow::editFunction(const QModelIndex &index)
{
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->functionView->model());
    if(model != 0) {
        QVariant item = model->data(index, Qt::UserRole);
        if(item.isValid()) {
            qDebug() << item;
            AbstractFunctionPtr f = item.value<AbstractFunctionPtr>();
            if(f->getType() == AbstractFunction::Function) {
                FunctionPtr func = qSharedPointerDynamicCast<Function>(f);
                if(func != 0) {
                    NewFunctionDialog* dialog = new NewFunctionDialog(func);
                    connect(dialog, SIGNAL(accepted()), this, SLOT(functionDialogAccepted()));
                    dialog->show();
                }
            } else if(f->getType() == AbstractFunction::HierarchicalFunction) {
                HierarchicalFunctionPtr function = qSharedPointerDynamicCast<HierarchicalFunction>(f);
                if(function != 0) {
                    HierarchicalFunctionDialog dialog(this);
                    dialog.setFunction(function);
                    if(dialog.exec() == QDialog::Accepted) {
                       newFunctionAvailable();
                    }
                }
            } else if(f->getType() == AbstractFunction::Integral) {
                IntegralFunctionPtr function = qSharedPointerDynamicCast<IntegralFunction>(f);
                if(function != 0) {
                    IntegralFunctionDialog dialog(this);
                    dialog.setFunction(function);
                    if(dialog.exec() == QDialog::Accepted) {
                        newFunctionAvailable();
                    }
                }
            } else if(f->getType() == AbstractFunction::Differential) {
                DifferentialFunctionPtr function = qSharedPointerDynamicCast<DifferentialFunction>(f);
                if(function != 0) {
                    DifferentialFunctionDialog dialog(function, this);
                    if(dialog.exec() == QDialog::Accepted) {
                        newFunctionAvailable();
                    }
                }
            }
        }
    }
}

void PlotControlWindow::editFunctionCurve()
{
    QModelIndex index = ui->functionView->currentIndex();
    AbstractFunctionPtr f = index.data(Qt::UserRole).value<AbstractFunctionPtr>();
    FunctionCurve *curve = f->getCurve();
    if(curve == 0) {
        curve = f->createCurve();
    }
    NewCurveDialog dialog;
    dialog.loadFromCurve(curve);
    if(dialog.exec() == QDialog::Accepted) {
        f->setCurve(curve);
    }
}

void PlotControlWindow::deleteSelectedCurve()
{
    qDebug() << "PlotControlWindow::deleteCurve()";

    // XXX: More appropriate call?
    newFunctionAvailable();
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->curveSelection->model());
    QModelIndex index = ui->curveSelection->currentIndex();
    Curve *c = index.data(Qt::UserRole).value<Curve *>();
    if(c != 0) {
        mCurveItems.removeOne(model->itemFromIndex(index));
        Singleton<CurveSingleton>::Instance().removeCurve(c);
        model->removeRow(index.row());
    }
}


void PlotControlWindow::functionDialogAccepted()
{
    qDebug() << "accepted!";
    newFunctionAvailable();
}

void PlotControlWindow::autoOrdinateChecked(bool checked)
{
    ui->autoOrdinate->setChecked(checked);
    ui->minOrdinateRange->setEnabled(!checked);
    ui->maxOrdinateRange->setEnabled(!checked);
}
