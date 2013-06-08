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

    // Connect PlotArea events
    connect( ui->plotConfigButton, SIGNAL(clicked()), this, SLOT(openConfigDialog()) );

    mConfigDialog = new PlotControlDialog(mName, this);
    mConfigDialog->setWindowTitle(tr("Configuring plot ")+mName);
}

PlotArea::~PlotArea()
{
    delete ui;
}



// ======================== SLOTS =======================================
void PlotArea::openConfigDialog()
{
    qDebug() << "Opening Graph control dialog";
    mConfigDialog->exec();
}

void PlotArea::plotConfigChanged()
{
    qDebug() << "Plot configuration changed";
    mConfigDialog->accept();
}
