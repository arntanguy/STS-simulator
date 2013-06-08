#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "plotwidget.h"
#include "settings.h"
#include "plotarea.h"

#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mPlotArea1 = new PlotArea;
    mPlotArea2 = new PlotArea;
    mPlotArea3 = new PlotArea;
    mPlotArea4 = new PlotArea;

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

    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(actionAbout(bool)));

    /* ui->plotWidget1->applySettings("plotWidget1");
    ui->plotWidget2->applySettings("plotWidget2");
    ui->plotWidget3->applySettings("plotWidget3");
    ui->plotWidget4->applySettings("plotWidget4"); */

}

MainWindow::~MainWindow()
{
    delete ui;
}


// ============== PRIVATE FUNCTIONS ================
void MainWindow::readSettings() {
    //QSettings settings;
    //settings.beginGroup("GlobalPlotOptions/legend");
    //settings.value("isEnabled", false).toBool();
    //move(settings.value("pos", QPoint(200, 200)).toPoint());
    //settings.endGroup();
}


// =============== SLOTS ===================
void MainWindow::actionAbout(bool)
{
    AboutDialog dialog(this);
    dialog.exec();
}
