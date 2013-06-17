#include "newcurvedialog.h"
#include "ui_newcurvedialog.h"
#include "curve.h"
#include "curvesingleton.h"

NewCurveDialog::NewCurveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewCurveDialog)
{
    ui->setupUi(this);
    mCurve = new Curve();

    init();

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

NewCurveDialog::~NewCurveDialog()
{
    delete ui;
}

void NewCurveDialog::init()
{
    ui->curvePenStyle->addItem(tr("Solid Line"), Qt::SolidLine);
    ui->curvePenStyle->addItem(tr("Dot Line"), Qt::DotLine);
    ui->curvePenStyle->addItem(tr("Dash Line"), Qt::DashLine);
    ui->curvePenStyle->addItem(tr("Dash and Dot Line"), Qt::DashDotLine);
    ui->curvePenStyle->addItem(tr("Dash Dot Dot Dash Line"), Qt::DashDotDotLine);
}

// ========================= SLOTS ============================
void NewCurveDialog::accept()
{
    mCurve->setTitle(ui->curveName->text());
    Qt::PenStyle pen = static_cast<Qt::PenStyle>(ui->curvePenStyle->itemData(ui->curvePenStyle->currentIndex()).toInt());
    mCurve->setPen(ui->curveColor->currentColor(), ui->curveThickness->value(), pen);
    Singleton<CurveSingleton>::Instance().addCurve(mCurve);
    QDialog::accept();
}
