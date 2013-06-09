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

    initFromConfig();


    connect(this->ui->buttonBox, SIGNAL(accepted()), parent, SLOT(plotConfigChanged()));
    connect(this->ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

PlotControlDialog::~PlotControlDialog()
{
    delete ui;
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

    QDialog::accept();
}
