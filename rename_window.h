#ifndef _RENAME_WINDOW_H_
#define _RENAME_WINDOW_H_

/*
** -- Qt Includes --
*/

#include <qlabel.h>
#include <qdialog.h>
#include <qlayout.h>
#include <qregexp.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qpushbutton.h>

/*
** -- Local Includes --
*/

#include <qtnqc_main.h>

/*
** We don't need a destructor since the dialog
** will be a child of a parent.
*/

class rename_window: public QDialog
{
  Q_OBJECT

 public:
  int isParentMainCanvas(void);
  void setObject(const object *);
  void setNameStr(const QString &);
  QString getNameStr(void);
  rename_window(QWidget *);
  ~rename_window();

 private:
  int parent_is_main_canvas;
  object *obj;
  QString name_str;
  QLineEdit *name;
  QPushButton *ok;
  QPushButton *help;
  QPushButton *cancel;

 private slots:
  void ok_cb(void);
  void help_cb(void);
  void close_cb(void);
};

#endif
