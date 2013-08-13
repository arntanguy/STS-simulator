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

    void saveIfNeeded() {
        if(mNeedsSaving)
            save();
        mNeedsSaving = false;
    }
signals:
    void valueChanged(QString var, double val);

public slots:
    void variableValueChanged(QString var, double val);
    void updateVariables();
    void configureAllSliders(double, double, double);

private:
    void init();
    void useFunction(const FunctionPtr& f);

private:
    FunctionPtr mFunction;
    Ui::FunctionVariablesWidget *ui;

    bool mNeedsSaving;

    QHBoxLayout *mVariabesLayout;
    QList<ValueSelector *> mValueSelectors;

};

#endif // FUNCTIONVARIABLESWIDGET_H
