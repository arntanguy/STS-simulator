#include "exportcurvesdialog.h"
#include "ui_exportcurvesdialog.h"

#include "functionssingleton.h"
#include "abstractfunction.h"
#include "functioncurve.h"
#include "typedefs.h"

#include <QStandardItemModel>
#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>

ExportCurvesDialog::ExportCurvesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportCurvesDialog)
{
    ui->setupUi(this);
    init();
}

ExportCurvesDialog::~ExportCurvesDialog()
{
    delete ui;
}

void ExportCurvesDialog::init()
{
    initCurves();

    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(chooseFile()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(rejected()));
}

void ExportCurvesDialog::initCurves()
{
    ui->curveSelection->setModel(new QStandardItemModel());
    qDebug() << "PlotControlWindow::newCurveAvailable()";
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->curveSelection->model());
    model->clear();
    //mCurveItems.clear();

    FunctionsSingleton *s = &Singleton<FunctionsSingleton>::Instance();
    QList<int> ids = s->getFunctionIds();
    int j=0;
    foreach(int id, ids) {
        AbstractFunctionPtr f = s->getFunctionById(id);
        if(f != 0 && (f->getType() == AbstractFunction::HierarchicalFunction || f->getType() == AbstractFunction::Differential || f->getType() == AbstractFunction::Integral)) {
            FunctionCurve *c=f->getCurve();
            if(c != 0) {
                QStandardItem *Item = new QStandardItem();
                Item->setCheckable( true );
                Item->setEditable(false);
                Item->setText(c->title().text());
                Item->setData(QVariant::fromValue(c), Qt::UserRole);
     //           mCurveItems.append(Item);
                model->setItem( j++, Item );
            }
        }
    }
}

void ExportCurvesDialog::chooseFile()
{
    // Load previously used directory when loading experimental data
    QSettings settings;
    QString startDir = settings.value("Save/exportDataDirectory", "").toString();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Choose export file"),
                                                    startDir,
                                                    tr("TXT (*.txt);;CSV (*.csv);;All Files (*.*);;All (*)"));
    if(!fileName.isEmpty()) {
        if(!(fileName.endsWith(".txt") || fileName.endsWith(".TXT") || fileName.endsWith(".csv") || fileName.endsWith(".CSV"))) {
            fileName = fileName + ".txt";
        }
        settings.setValue("Save/exportDataDirectory", QFileInfo(fileName).absoluteDir().absolutePath());
        ui->filePath->setText(fileName);
    }
}

void ExportCurvesDialog::accept()
{
    QDialog::accept();
}
