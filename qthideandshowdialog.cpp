#include <cassert>
#include "qthideandshowdialog.h"

QtHideAndShowDialog::QtHideAndShowDialog(QWidget *parent)
  : QDialog(parent),
    m_show_child(false)
{

}

void QtHideAndShowDialog::close_child()
{
  m_show_child = false;
}

void QtHideAndShowDialog::closeEvent(QCloseEvent *)
{
  emit close_me();
}


void QtHideAndShowDialog::ShowChild(QtHideAndShowDialog * const dialog)
{
  assert(dialog);
  this->hide();
  QObject::connect(dialog,SIGNAL(close_me()),this,SLOT(close_child()));
  m_show_child = true;
  while (m_show_child)
  {
    dialog->exec();
  }
  this->show();
}
