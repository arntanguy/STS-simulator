#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
