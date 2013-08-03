#ifndef QTHIDEANDSHOWDIALOG_H
#define QTHIDEANDSHOWDIALOG_H

#include <QDialog>

class QtHideAndShowDialog : public QDialog
{
    Q_OBJECT

public:
  explicit QtHideAndShowDialog(QWidget *parent = 0);

  ///Virtual destructor as this is a base class
  virtual ~QtHideAndShowDialog() {}

  ///Show a child
  void ShowChild(QtHideAndShowDialog * const dialog);

protected:
  ///Shows a child until it emits a close_me signal
  bool m_show_child;

  ///closeEvent that emits the close_me signal
  void closeEvent(QCloseEvent *);

signals:
  ///Emit the closeEvent of this dialog
  void close_me();

protected slots:
  ///Slot that needs to be called when a child signals close_me
  void close_child();

};

#endif // QTHIDEANDSHOWDIALOG_H
