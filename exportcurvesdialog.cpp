#include "exportcurvesdialog.h"
#include "ui_exportcurvesdialog.h"

#include "functionssingleton.h"
#include "abstractfunction.h"
#include "functioncurve.h"
#include "typedefs.h"
#include "csvfilewriter.h"

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
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void ExportCurvesDialog::initCurves()
{
    ui->curveSelection->setModel(new QStandardItemModel());
    qDebug() << "PlotControlWindow::newCurveAvailable()";
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->curveSelection->model());
    model->clear();
    //mCurveItems.clear();

    mItems.clear();
    FunctionsSingleton *s = &Singleton<FunctionsSingleton>::Instance();
    QList<int> ids = s->getFunctionIds();
    int j=0;
    foreach(int id, ids) {
        AbstractFunctionPtr f = s->getFunctionById(id);
        if(f != 0 && (f->getType() == AbstractFunction::HierarchicalFunction || f->getType() == AbstractFunction::Differential || f->getType() == AbstractFunction::Integral)) {
            FunctionCurve *c=f->getCurve();
            if(c != 0 && c->isAttached()) {
                QStandardItem *Item = new QStandardItem();
                Item->setCheckable( true );
                Item->setEditable(false);
                Item->setText(c->title().text());
                Item->setData(QVariant::fromValue(c), Qt::UserRole);
     //           mCurveItems.append(Item);
                model->setItem( j++, Item );
                mItems.append(Item);
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

bool ExportCurvesDialog::exportData(const QString &file)
{
    QStringList name;
    name << "V";
    QVector< QVector<double> > xData;
    QVector< QVector<double> > yData;

    foreach(QStandardItem *item, mItems) {
        //manageFunctionCurveFromItem(item);
        FunctionCurve *c =  item->data(Qt::UserRole).value<FunctionCurve *>();
        if(c != 0) {
            if(item->isCheckable() && item->checkState() == Qt::Checked) {
                xData.append(c->getXData());
                yData.append(c->getYData());
                name.append(c->title().text());
            }
        }
    }

    int size = xData.size();
    if(size > 0) {
        int nPoints = xData[0].size();
        qDebug() << "size >0";
        // Check if correct number of points
        for(int i=0; i<xData.size(); i++) {
            if(xData[i].size() != nPoints && yData[i].size() != nPoints) {
                return false;
            }
        }
        qDebug() << "correct number";
        // Check if x points are the same
        for(int i=0; i<xData.size(); i++) {
                for(int j=0; j<xData[i].size(); j++) {
                    if(xData[i][j] != xData[0][j]) {
                        return false;
                    }
                }
        }
        qDebug() << "same x";
        QVector<double> xx = xData[0];
        QVector<QStringList> exportD(nPoints);
        // Create export lines
        for(int i=0; i<xx.size(); i++) {
            exportD[i] << QString::number(xx[i]);
            for(int j=0; j<yData.size(); j++) {
                exportD[i] << QString::number(yData[j][i]);
            }
        }

        CSVFileWriter w("\t");
        if(w.openFile(file)) {
            w.writeLine(name);
            foreach(QStringList l, exportD) {
                w.writeLine(l);
            }
        }
    }
}

void ExportCurvesDialog::accept()
{
    exportData(ui->filePath->text());
    QDialog::accept();
}
