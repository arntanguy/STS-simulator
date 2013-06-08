#include "plotcontroldialog.h"
#include "ui_plotcontroldialog.h"

PlotControlDialog::PlotControlDialog(const QString &plotName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotControlDialog)
{
    ui->setupUi(this);

    mPlotName = plotName;

    connect(this->ui->buttonBox, SIGNAL(accepted()), parent, SLOT(plotConfigChanged()));
}

PlotControlDialog::~PlotControlDialog()
{
    delete ui;
}
