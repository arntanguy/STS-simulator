#include <QAction>
#include "plotarea.h"
#include "ui_plotarea.h"
#include "plotcontroldialog.h"

PlotArea::PlotArea(const QString &name, const unsigned int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotArea)
{
    ui->setupUi(this);

    setId(id);
    mName = name;

    ui->plotWidget->setTitle(mName);
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
    PlotControlDialog mConfigDialog(getId(), this);
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

    ui->plotWidget->loadFromSettings();
}
