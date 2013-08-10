#ifndef FUNCTIONVARIABLESWIDGET_H
#define FUNCTIONVARIABLESWIDGET_H

#include <QWidget>
#include <QHBoxLayout>

#include "typedefs.h"

namespace Ui {
class FunctionVariablesWidget;
}

class Function;
class ValueSelector;

class FunctionVariablesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FunctionVariablesWidget(QWidget *parent = 0);

    void setFunction(const FunctionPtr& f);
    void save();

signals:
    void valueChanged(QString var, double val);

public slots:
    void variableValueChanged(QString var, double val);
    void updateVariables();

private:
    void init();
    void useFunction(const FunctionPtr& f);

private:
    FunctionPtr mFunction;
    Ui::FunctionVariablesWidget *ui;

    QHBoxLayout *mVariabesLayout;
    QList<ValueSelector *> mValueSelectors;

};

#endif // FUNCTIONVARIABLESWIDGET_H
