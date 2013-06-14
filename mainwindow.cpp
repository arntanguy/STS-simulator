#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "plotwidget.h"
#include "plotarea.h"
#include "csvexperimentaldatareader.h"
#include "curve.h"

#include <QMdiSubWindow>
#include <QFileDialog>
#include <QColor>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mPlotArea1 = new PlotArea("Plot1");
    mPlotArea2 = new PlotArea("Plot2");
    mPlotArea3 = new PlotArea("Plot3");
    mPlotArea4 = new PlotArea("Plot4");

    QVBoxLayout *l1 = new QVBoxLayout;
    l1->addWidget(mPlotArea1);
    ui->widget1->setLayout(l1);

    QVBoxLayout *l2 = new QVBoxLayout;
    l2->addWidget(mPlotArea2);
    ui->widget2->setLayout(l2);

    QVBoxLayout *l3 = new QVBoxLayout;
    l3->addWidget(mPlotArea3);
    ui->widget3->setLayout(l3);

    QVBoxLayout *l4 = new QVBoxLayout;
    l4->addWidget(mPlotArea4);
    ui->widget4->setLayout(l4);

    // Show main window maximized
    showMaximized();

    // Connect Menu Events
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(actionAbout(bool)));
    connect(ui->actionLoad_Experimental_Data, SIGNAL(triggered(bool)), this, SLOT(actionLoadExperimentalData(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


// ============== PRIVATE FUNCTIONS ================
void MainWindow::readSettings() {
}


// =============== SLOTS ===================
void MainWindow::actionAbout(bool)
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::actionLoadExperimentalData(bool)
{
    // Load previously used directory when loading experimental data
    QString startDir = mSettings.value("Save/experimentalDataDirectory", "").toString();

    QString fileName = QFileDialog::getOpenFileName(this,
     tr("Open Experimental Data"), startDir, tr("Experimental Data Files(*.csv *.txt);;All Files (*.*)"));


    CSVExperimentalDataReader reader;
    reader.parseFile(fileName, "\t");


    const char *colors[] =
    {
        "LightSalmon",
        "SteelBlue",
        "Yellow",
        "Fuchsia",
        "PaleGreen",
        "PaleTurquoise",
        "Cornsilk",
        "HotPink",
        "Peru",
        "Maroon"
    };
    const int numColors = sizeof( colors ) / sizeof( colors[0] );
    Curve *curve = new Curve( QString("Experimental DZ2") );
    curve->setPen( QColor( colors[ numColors ] ), 2 );
    curve->setSamples(reader.getColumn(0, 1000).getData(), reader.getColumn(4, 1000).getData());
    curve->attach( mPlotArea3->getPlotWidget() );
    mPlotArea3->getPlotWidget()->replot();



    // Save currently used directory for later use
    if(!fileName.isNull()) {
        mSettings.setValue("Save/experimentalDataDirectory", QFileInfo(fileName).absoluteDir().absolutePath());
    }
}
