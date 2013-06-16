#ifndef PROJECTDIALOG_H
#define PROJECTDIALOG_H

#include <QDialog>

namespace Ui {
class ProjectDialog;
}

class ProjectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProjectDialog(QWidget *parent = 0);
    ~ProjectDialog();
    
private:
    void initFromConfig();

private:
    Ui::ProjectDialog *ui;

public slots:
    void slotOpenProject();

signals:
    void openProject(QString&);
};

#endif // PROJECTDIALOG_H
