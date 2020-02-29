#ifndef _LIST_OBJECTS_H_
#define _LIST_OBJECTS_H_

/*
** -- Qt Includes --
*/

#include <qlabel.h>
#include <qtable.h>
#include <qdialog.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qpushbutton.h>

/*
** -- Local Includes --
*/

#include <qtnqc_main.h>

/*
** We don't need a destructor since the dialog
** will be a child of a parent.
*/

class list_objects: public QDialog
{
  Q_OBJECT

 public:
  list_objects(QWidget *, editor *);
  ~list_objects();
  void fill(void);
  void clear(void);

 private:
  editor *edit;
  QTable *table;
  QPushButton *cancel;
  QPushButton *remove;
  QPushButton *goto_loc;
  void getXY(int *, int *);

 private slots:
  void goto_cb(void);
  void close_cb(void);
  void delete_cb(void);
};

#endif
