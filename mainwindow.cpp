#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "plotwidget.h"
#include "settings.h"

#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Show main window maximized
    showMaximized();

    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(actionAbout(bool)));


    // General graph settings
    // TODO: provide a control panel to change these settings
    Settings settings;
    settings.legend.isEnabled = false;
    settings.legend.position = QwtPlot::BottomLegend;

    settings.legendItem.isEnabled = true;
    settings.legendItem.numColumns = 1;
    settings.legendItem.alignment = Qt::AlignRight | Qt::AlignTop;
    settings.legendItem.backgroundMode = 0;
    settings.legendItem.size = ui->plotWidget1->canvas()->font().pointSize();

    settings.curve.numCurves = 3;
    settings.curve.title = "Curve";

    ui->plotWidget1->applySettings(settings);
    ui->plotWidget2->applySettings(settings);
    ui->plotWidget3->applySettings(settings);
    ui->plotWidget4->applySettings(settings);

}

MainWindow::~MainWindow()
{
    delete ui;
}



// =============== SLOTS ===================
void MainWindow::actionAbout(bool)
{
    AboutDialog dialog(this);
    dialog.exec();
}
