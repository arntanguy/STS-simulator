#include "exportcurvesdialog.h"
#include "ui_exportcurvesdialog.h"

#include "functionssingleton.h"
#include "abstractfunction.h"
#include "curve.h"
#include "typedefs.h"

#include <QStandardItemModel>

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
}

void ExportCurvesDialog::initCurves()
{
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
            Curve *c=0;
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

