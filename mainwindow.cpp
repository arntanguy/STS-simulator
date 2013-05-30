#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "plotwidget.h"

#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMdiSubWindow *subWindow1 = new QMdiSubWindow;
    PlotWidget *widget1 = new PlotWidget;
    subWindow1->setWidget(widget1);
    subWindow1->setAttribute(Qt::WA_DeleteOnClose);
    subWindow1->setWindowTitle("Plot 1");
    subWindow1->showMaximized();
    ui->mdiArea->addSubWindow(subWindow1);
    ui->mdiArea->setActiveSubWindow(subWindow1);

    QMdiSubWindow *subWindow2 = new QMdiSubWindow;
    PlotWidget *widget2 = new PlotWidget;
    subWindow2->setWidget(widget2);
    subWindow2->setAttribute(Qt::WA_DeleteOnClose);
    subWindow2->setWindowTitle("Plot 2");
    subWindow2->showMaximized();
    ui->mdiArea->addSubWindow(subWindow2);
    ui->mdiArea->setActiveSubWindow(subWindow2);

    ui->mdiArea->tileSubWindows();

    // Show main window maximized
    showMaximized();

    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(actionAbout(bool)));
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
