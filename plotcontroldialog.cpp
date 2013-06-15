#include <QSettings>
#include <QDebug>
#include "plotcontroldialog.h"
#include "ui_plotcontroldialog.h"
#include <qwt_plot.h>

PlotControlDialog::PlotControlDialog(const QString &plotName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotControlDialog)
{
    ui->setupUi(this);
    mPlotName = plotName;

    init();
    initFromConfig();

    connect(this->ui->buttonBox, SIGNAL(accepted()), parent, SLOT(plotConfigChanged()));
    connect(this->ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(this->ui->autoAbscissa, SIGNAL(toggled(bool)), this, SLOT(autoAbscissaChecked(bool)));
    connect(this->ui->autoOrdinate, SIGNAL(toggled(bool)), this, SLOT(autoOrdinateChecked(bool)));
}

PlotControlDialog::~PlotControlDialog()
{
    delete ui;
}

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
}

/*!
 * \brief PlotControlDialog::initFromConfig
 *  Initialize from saved configuration (QSettings)
 */
void PlotControlDialog::initFromConfig()
{
    qDebug() << "PlotControlDialog::initFromConfig()";
    mSettings.beginGroup("Plot/"+mPlotName+"/legend");
    ui->legendCheckBox->setChecked(mSettings.value("isEnabled", false).toBool());
    int index = ui->legendPositionComboBox->findData(mSettings.value("position", Qt::AlignBottom).toInt());
    if(index != -1)
        ui->legendPositionComboBox->setCurrentIndex(index);
    mSettings.endGroup();

    mSettings.beginGroup("Plot/"+mPlotName+"/legendItem");
    ui->legendItemCheckBox->setChecked(mSettings.value("isEnabled", false).toBool());
    QVariant align = static_cast<int>(Qt::AlignRight);
    index = ui->legendItemHorizontalPositionComboBox->findData(mSettings.value("horizontalPosition", align).toInt());
    if(index != -1)
        ui->legendItemHorizontalPositionComboBox->setCurrentIndex(index);
    align = static_cast<int>(Qt::AlignTop);
    index = ui->legendItemVerticalPositionComboBox->findData(mSettings.value("verticalPosition", align).toInt());
    if(index != -1)
        ui->legendItemVerticalPositionComboBox->setCurrentIndex(index);
    mSettings.endGroup();

    mSettings.beginGroup("Plot/"+mPlotName+"/info");
    ui->titleLineEdit->setText(mSettings.value("title", mPlotName).toString());
    ui->horizontalAxisLineEdit->setText(mSettings.value("horizontalAxisName", "X Axis").toString());
    ui->verticalAxisLineEdit->setText(mSettings.value("verticalAxisName", "X Axis").toString());
    mSettings.endGroup();

    mSettings.beginGroup("Plot/"+mPlotName+"/axisScale");
    index = ui->horizontalAxisScale->findData(mSettings.value("horizontalAxisScale", "linear").toString());
    if(index != -1)
        ui->horizontalAxisScale->setCurrentIndex(index);
    index = ui->verticalAxisScale->findData(mSettings.value("verticalAxisScale", "linear").toString());
    if(index != -1)
        ui->verticalAxisScale->setCurrentIndex(index);
    mSettings.endGroup();

    mSettings.beginGroup("Plot/"+mPlotName+"/precision");
    ui->plotResolution->setValue(mSettings.value("resolution", 1000).toDouble());
    mSettings.endGroup();




    // ========= Plot range ===========
    mSettings.beginGroup("Plot/"+mPlotName+"/range");
    autoAbscissaChecked(mSettings.value("autoAbscissa", true).toBool());
    ui->minAbscissaRange->setValue(mSettings.value("minAbscissa", 0).toDouble());
    ui->maxAbscissaRange->setValue(mSettings.value("maxAbscissa", 1000).toDouble());

    autoOrdinateChecked(mSettings.value("autoOrdinate", true).toBool());
    ui->minOrdinateRange->setValue(mSettings.value("minOrdinate", 0).toDouble());
    ui->maxOrdinateRange->setValue(mSettings.value("maxOrdinate", 1000).toDouble());
    mSettings.endGroup();
}

/*!
 * \brief PlotControlDialog::accept
 *  Saves all the parameters set within the config dialog (QSettings)
 *  Discards config dialog
 */
void PlotControlDialog::accept()
{
    qDebug() << "PlotControlDialog::accept()";
    mSettings.beginGroup("Plot/"+mPlotName+"/legend");
    mSettings.setValue("isEnabled", ui->legendCheckBox->isChecked());
    mSettings.setValue("position", ui->legendPositionComboBox->itemData(ui->legendPositionComboBox->currentIndex()));
    mSettings.endGroup();

    mSettings.beginGroup("Plot/"+mPlotName+"/legendItem");
    mSettings.setValue("isEnabled", ui->legendItemCheckBox->isChecked());
    int horizontalPos = ui->legendItemHorizontalPositionComboBox->itemData(ui->legendItemHorizontalPositionComboBox->currentIndex()).toInt();
    int verticalPos = ui->legendItemVerticalPositionComboBox->itemData(ui->legendItemVerticalPositionComboBox->currentIndex()).toInt();
    Qt::Alignment aX = static_cast<Qt::Alignment>(horizontalPos);
    Qt::Alignment aY = static_cast<Qt::Alignment>(verticalPos);
    mSettings.setValue("horizontalPosition", horizontalPos);
    mSettings.setValue("verticalPosition", verticalPos);
    mSettings.setValue("alignment", static_cast<int>(aX|aY));
    mSettings.setValue("numCurves", 3);
    mSettings.endGroup();

    mSettings.beginGroup("Plot/"+mPlotName+"/range");
    mSettings.setValue("autoAbscissa", ui->autoAbscissa->isChecked());
    mSettings.setValue("minAbscissa", ui->minAbscissaRange->value());
    mSettings.setValue("maxAbscissa", ui->maxAbscissaRange->value());
    mSettings.setValue("autoOrdinate", ui->autoOrdinate->isChecked());
    mSettings.setValue("minOrdinate", ui->minOrdinateRange->value());
    mSettings.setValue("maxOrdinate", ui->maxOrdinateRange->value());
    mSettings.endGroup();

    mSettings.beginGroup("Plot/"+mPlotName+"/info");
    mSettings.setValue("title", ui->titleLineEdit->text());
    mSettings.setValue("horizontalAxisName", ui->horizontalAxisLineEdit->text());
    mSettings.setValue("verticalAxisName", ui->verticalAxisLineEdit->text());
    mSettings.endGroup();

    mSettings.beginGroup("Plot/"+mPlotName+"/axisScale");
    mSettings.setValue("horizontalAxisScale", ui->horizontalAxisScale->itemData(ui->horizontalAxisScale->currentIndex()));
    mSettings.setValue("verticalAxisScale", ui->verticalAxisScale->itemData(ui->verticalAxisScale->currentIndex()));
    mSettings.endGroup();

    mSettings.beginGroup("Plot/"+mPlotName+"/precision");
    mSettings.setValue("resolution", ui->plotResolution->value());
    mSettings.endGroup();

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
