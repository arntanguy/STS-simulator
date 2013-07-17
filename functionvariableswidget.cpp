#include "functionvariableswidget.h"
#include "ui_functionvariableswidget.h"

#include "projectsingleton.h"
#include "function.h"
#include "valueselector.h"
#include "helperfunctions.h"

#include <QDebug>
#include <QLabel>

FunctionVariablesWidget::FunctionVariablesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionVariablesWidget)
{
    ui->setupUi(this);
    mVariabesLayout = new QVBoxLayout();
    QWidget *w = new QWidget(this);
    w->setLayout(mVariabesLayout);
    ui->variablesScrollArea->setWidget(w);
}

void FunctionVariablesWidget::setFunction(Function *f)
{
    useFunction(f);
}

// =============================== PRIVATE ====================================
void FunctionVariablesWidget::useFunction(Function *f)
{
    mFunction = f;
    connect(f, SIGNAL(expressionChanged()), this, SLOT(updateVariables()));
    updateVariables();
}



// =============================== SLOTS ==================================
void FunctionVariablesWidget::updateVariables()
{
    HelperFunctions::clearLayout(mVariabesLayout);
    mValueSelectors.clear();
    QStringList variables = mFunction->getVariableFactory()->getVariableNames();
    qDebug() << "Function variables: " << variables;
    foreach(QString var, variables) {
        // If it's not the function variable
        if(var != mFunction->getVariable()) {
            qDebug() << "FunctionVariablesWidget::useFunction() - varible " << var << " detected";
            ValueSelector *valueSelector = new ValueSelector(var, mFunction->getVariableFactory()->getVariableAddress(var), this);
            connect(valueSelector, SIGNAL(valueChanged(QString,double)), this, SLOT(variableValueChanged(QString, double)));
            // Creates a widget to control it
            mVariabesLayout->addWidget(valueSelector);
            mValueSelectors.append(valueSelector);
        }
    }
}

void FunctionVariablesWidget::variableValueChanged(QString var, double val)
{
    qDebug() << "Variable value changed: " << var << ": " << val;
    if(mFunction != 0) {
        qDebug() << "Eval: " << mFunction->getExpression() << " for value V=2 " << mFunction->compute(2);
        mFunction->updateLinkedCurve(var, val);
        emit valueChanged(var, val);
    }
}

void FunctionVariablesWidget::save(const QString &group)
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup(group);

    settings->beginGroup("Variables");
    settings->remove("");
    qDebug() << "Saving variables for function "<<mFunction->getName() << " in " << settings->group();
    foreach(ValueSelector *vSelector, mValueSelectors) {
        vSelector->save(settings);
    }
    settings->endGroup();

    settings->endGroup();
}
