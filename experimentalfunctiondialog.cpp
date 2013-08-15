#include "experimentalfunctiondialog.h"
#include "ui_experimentalfunctiondialog.h"

#include "datasingleton.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <QSettings>
#include "experimentaldata.h"
#include "typedefs.h"

ExperimentalFunctionDialog::ExperimentalFunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExperimentalFunctionDialog)
{
    ui->setupUi(this);
    init();

    connect(ui->dataLoadButton, SIGNAL(clicked()), this, SLOT(loadDataFile()));
    connect(ui->dataLoaded, SIGNAL(currentIndexChanged(int)), this, SLOT(dataFileChanged(int)));
}

ExperimentalFunctionDialog::~ExperimentalFunctionDialog()
{
    delete ui;
}

void ExperimentalFunctionDialog::init()
{
    ui->dataTable->setEditTriggers(QTableView::NoEditTriggers);

    //Loads the data first
    QStringList dataPaths = Singleton<DataSingleton>::Instance().getExperimentalDataPaths();
    ui->dataLoaded->clear();
    if(!dataPaths.isEmpty()) {
        foreach(QString data, dataPaths) {
            ui->dataLoaded->addItem(QFileInfo(data).baseName(), data);
        }
        dataFileChanged(ui->dataLoaded->currentIndex());
    }
}

void ExperimentalFunctionDialog::dataFileChanged(int index)
{
    QString fileName = ui->dataLoaded->itemData(index).toString();
    qDebug() << "Data file changed: " << fileName;


    // Fill in the QComboBox for choosing abscissia and ordinate
    QSharedPointer<Data> data = Singleton<DataSingleton>::Instance().getData(fileName);
    if(data != 0) {
        QStringList columns = data->getAvailableColumns();
        foreach(QString column, columns) {
            ui->data->addItem(column, column);
        }
        // Autoset abscissia to DZ1 column
        int ind = ui->data->findData("DZ1");
        if(ind != -1) {
            ui->data->setCurrentIndex(ind);
        }

        // ========= Fill in the values in the tablewidget
        if(columns.length() > 0) {
            QStandardItemModel *model = new QStandardItemModel(this);
            model->setColumnCount(columns.length());
            for(int i=0; i<columns.length(); i++) {
                model->setHorizontalHeaderItem(i, new QStandardItem(columns[i]));
                DataColumn<double> col = data->getColumn(columns[i]);
                QVector<double> val = col.getData();
                for(int j=0; j<val.size(); j++) {
                    model->setItem(j, i, new QStandardItem(QString::number(val[j])));
                }
            }
            ui->dataTable->setModel(model);
        }
    }
}

void ExperimentalFunctionDialog::accept()
{
    qDebug() << "NewCurveDialog::accept()";
    bool mayClose = false;
    QString title = ui->name->text();
    if(!mEdit /*&& Singleton<FunctionsSingleton>::Instance().functionNameExists(title)*/) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Name Conflict"), tr("Another curve with the name ") + title + tr(" already exists. Do you want to modify the name?"), QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            mayClose = false;
            return;
        } else {
            mayClose = true;
        }
    } else {
        mayClose = true;
    }

    // Set function data
    QString fileName = ui->dataLoaded->itemData(ui->dataLoaded->currentIndex()).toString();
    if(!fileName.isEmpty()) {
            QString data = ui->data->itemData(ui->data->currentIndex()).toString();
            if(!data.isEmpty()) {
                // Set function data
            }
    }


    if(mayClose)
        QDialog::accept();
}

void ExperimentalFunctionDialog::loadDataFile()
{
    // Load previously used directory when loading experimental data
    QSettings mSettings;
    QString startDir = mSettings.value("Save/experimentalDataDirectory", "").toString();

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Experimental Data"), startDir, tr("Experimental Data Files(*.csv *.txt);;All Files (*.*)"));


    int index = ui->dataLoaded->findData(fileName);
    if(index != -1) {
        QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Data file already loaded"),
                                tr("This data file has already been loaded. If you have modified it in an external program, it might need to be loaded again.\n"
                                   "Do you want to load ")+fileName+tr(" again?"),
                                QMessageBox::Yes | QMessageBox::No);
        if(ret == QMessageBox::Yes) {
            DataPtr data = Singleton<DataSingleton>::Instance().getData(fileName);
            QSharedPointer<ExperimentalData> d = qSharedPointerDynamicCast<ExperimentalData>(data);
            if(d != 0) {
                d->loadFromFile(fileName);
            }
        }
    }
    else {
        ExperimentalData *data = new ExperimentalData();
        data->loadFromFile(fileName);
        Singleton<DataSingleton>::Instance().addData(data);
        ui->dataLoaded->addItem(QFileInfo(fileName).baseName(), fileName);
    }

    // Save currently used directory for later use
    if(!fileName.isNull()) {
        mSettings.setValue("Save/experimentalDataDirectory", QFileInfo(fileName).absoluteDir().absolutePath());
    }
}
