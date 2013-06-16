#include <QAction>
#include "plotarea.h"
#include "ui_plotarea.h"
#include "plotcontroldialog.h"

PlotArea::PlotArea(const QString &name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotArea)
{
    mName = name;

    ui->setupUi(this);
    ui->plotWidget->setTitle(mName);
    ui->plotWidget->applySettings(mName);
    ui->plotWidget->setName(mName);

    // Connect PlotArea events
    connect( ui->plotConfigButton, SIGNAL(clicked()), this, SLOT(openConfigDialog()) );

}

PlotArea::~PlotArea()
{
    delete ui;
}

// ======================== PUBLIC FUNCTIONS ============================
PlotWidget *PlotArea::getPlotWidget()
{
    return ui->plotWidget;
}

// ======================== SLOTS =======================================
void PlotArea::openConfigDialog()
{
    PlotControlDialog mConfigDialog(mName, this);
    mConfigDialog.setWindowTitle(tr("Configuring plot ")+mName);
    qDebug() << "Opening Graph control dialog";
    mConfigDialog.exec();
}

/*!
 * \brief PlotArea::plotConfigChanged
 *  Applies the new configuration.
 */
void PlotArea::plotConfigChanged()
{
    qDebug() << "Plot configuration changed";

    ui->plotWidget->applySettings(mName);
}
