#ifndef FUNCTIONVARIABLESWIDGET_H
#define FUNCTIONVARIABLESWIDGET_H

#include <QWidget>
#include <QVBoxLayout>


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

    void setFunction(Function *f);
    void save();

signals:
    void valueChanged(QString var, double val);

public slots:
    void variableValueChanged(QString var, double val);
    void updateVariables();

private:
    void useFunction(Function *f);

private:
    Function *mFunction;
    Ui::FunctionVariablesWidget *ui;

    QVBoxLayout *mVariabesLayout;
    QList<ValueSelector *> mValueSelectors;

};

#endif // FUNCTIONVARIABLESWIDGET_H
