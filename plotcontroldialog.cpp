#include <QSettings>
#include <QDebug>
#include "plotcontroldialog.h"
#include "ui_plotcontroldialog.h"

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
}

PlotControlDialog::~PlotControlDialog()
{
    delete ui;
}

void PlotControlDialog::init()
{
    ui->horizontalAxisScale->addItem(tr("Linear"), "linear");
    ui->horizontalAxisScale->addItem(tr("Log10"), "log10");
    ui->verticalAxisScale->addItem(tr("Linear"), "linear");
    ui->verticalAxisScale->addItem(tr("Log10"), "log10");
}

void PlotControlDialog::initFromConfig()
{
    qDebug() << "PlotControlDialog::initFromConfig()";
    mSettings.beginGroup("Plot/"+mPlotName+"/legend");
    ui->legendCheckBox->setChecked(mSettings.value("isEnabled", false).toBool());
    mSettings.endGroup();

    mSettings.beginGroup("Plot/"+mPlotName+"/legendItem");
    ui->legendItemCheckBox->setChecked(mSettings.value("isEnabled", false).toBool());
    mSettings.endGroup();

    mSettings.beginGroup("Plot/"+mPlotName+"/info");
    ui->titleLineEdit->setText(mSettings.value("title", mPlotName).toString());
    ui->horizontalAxisLineEdit->setText(mSettings.value("horizontalAxisName", "X Axis").toString());
    ui->verticalAxisLineEdit->setText(mSettings.value("verticalAxisName", "X Axis").toString());
    mSettings.endGroup();

    mSettings.beginGroup("Plot/"+mPlotName+"/axisScale");
    int index = ui->horizontalAxisScale->findData(mSettings.value("horizontalAxisScale", "linear").toString());
    if(index != -1)
        ui->horizontalAxisScale->setCurrentIndex(index);
    index = ui->verticalAxisScale->findData(mSettings.value("verticalAxisScale", "linear").toString());
    if(index != -1)
        ui->verticalAxisScale->setCurrentIndex(index);
    mSettings.endGroup();
}

void PlotControlDialog::accept()
{
    qDebug() << "PlotControlDialog::accept()";
    mSettings.beginGroup("Plot/"+mPlotName+"/legend");
    mSettings.setValue("isEnabled", ui->legendCheckBox->isChecked());
    mSettings.endGroup();

    mSettings.beginGroup("Plot/"+mPlotName+"/legendItem");
    mSettings.setValue("isEnabled", ui->legendItemCheckBox->isChecked());
    mSettings.setValue("numCurves", 3);
    mSettings.endGroup();

    mSettings.beginGroup("Plot/"+mPlotName+"/range");
    mSettings.setValue("min", ui->minRangeValue->value());
    mSettings.setValue("max", ui->maxRangeValue->value());
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

    QDialog::accept();
}
