#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

#include <qwt_plot.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Show main window maximized
    showMaximized();

    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(actionAbout(bool)));

    QwtPlot *plot = new QwtPlot(QwtText("Demo"), this);
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
