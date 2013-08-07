#include <QAction>
#include "plotarea.h"
#include "ui_plotarea.h"
#include "plotcontrolwindow.h"

PlotArea::PlotArea(const QString &name, const unsigned int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotArea)
{
    ui->setupUi(this);

    setId(id);
    mName = name;

    ui->plotWidget->setTitle(mName);
    ui->plotWidget->setName(mName);

    mConfigWindow = new PlotControlWindow(id, this);
    mConfigWindow->setWindowTitle(tr("Configuring plot ")+mName);

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

void PlotArea::setId(unsigned int id)
{
    ui->plotWidget->setId(id);
}
unsigned int PlotArea::getId() const
{
    return ui->plotWidget->getId();
}

// ======================== SLOTS =======================================
void PlotArea::openConfigDialog()
{
    qDebug() << "Opening Graph control dialog";
    mConfigWindow->newCurveAvailable();
    mConfigWindow->newFunctionAvailable();
    mConfigWindow->show();
}

/*!
 * \brief PlotArea::plotConfigChanged
 *  Applies the new configuration.
 */
void PlotArea::plotConfigChanged()
{
    qDebug() << "Plot configuration changed";

    ui->plotWidget->loadFromSettings();
}
