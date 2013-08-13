#include "functionvariableswidget.h"
#include "ui_functionvariableswidget.h"

#include "projectsingleton.h"
#include "function.h"
#include "valueselector.h"
#include "helperfunctions.h"
#include "variablefactory.h"
#include "globalsettingssingleton.h"

#include <QDebug>
#include <QLabel>

FunctionVariablesWidget::FunctionVariablesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionVariablesWidget)
{
    ui->setupUi(this);

    init();
}

void FunctionVariablesWidget::init()
{
    mVariabesLayout = new QHBoxLayout();
    QWidget *w = new QWidget(this);
    w->setLayout(mVariabesLayout);
    ui->variablesScrollArea->setWidget(w);
}

void FunctionVariablesWidget::setFunction(const FunctionPtr &f)
{
    useFunction(f);
}

// =============================== PRIVATE ====================================
void FunctionVariablesWidget::useFunction(const FunctionPtr &f)
{
    mFunction = f;
    connect(f.data(), SIGNAL(expressionChanged()), this, SLOT(updateVariables()));
    updateVariables();
}



// =============================== SLOTS ==================================
void FunctionVariablesWidget::updateVariables()
{
    HelperFunctions::clearLayout(mVariabesLayout);
    mValueSelectors.clear();
    if(mFunction != 0) {
        QStringList variables = mFunction->getVariableFactory()->getVariableNames();
        qDebug() << "Function variables: " << variables;
        foreach(QString var, variables) {
            // If it's not the function variable
            if(var != mFunction->getVariable()) {
                qDebug() << "FunctionVariablesWidget::useFunction() - variable " << var << " detected";
                ValueSelector *valueSelector = new ValueSelector(var, mFunction, this);
                connect(valueSelector, SIGNAL(valueChanged(QString,double)), this, SLOT(variableValueChanged(QString, double)));
                connect(valueSelector, SIGNAL(configureAllSliders(double, double, double)), this, SLOT(configureAllSliders(double, double, double)));
                valueSelector->loadFromConfig();
                // Creates a widget to control it
                mVariabesLayout->addWidget(valueSelector);
                mValueSelectors.append(valueSelector);
            }
        }
    }
}

void FunctionVariablesWidget::variableValueChanged(QString var, double val)
{
    qDebug() << "Variable value changed: " << var << ": " << val;
    if(mFunction != 0) {
        mFunction->updateLinkedCurve(var, val, true);
        emit valueChanged(var, val);
    }
}

void FunctionVariablesWidget::save()
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Variables/"+QString::number(mFunction->getId()));

    settings->remove("");
    qDebug() << "Saving variables for function "<<mFunction->getName() << " in " << settings->group();
    foreach(ValueSelector *vSelector, mValueSelectors) {
        vSelector->save();
    }

    settings->endGroup();
    settings->sync();
}

void FunctionVariablesWidget::configureAllSliders(double min, double max, double step)
{
    foreach(ValueSelector *s, mValueSelectors) {
        s->setRange(min, max, step);
    }
}
