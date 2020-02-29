#ifndef _OBJECT_H_
#define _OBJECT_H_

/*
** -- Qt Classes --
*/

class QCanvasView;
class QMainWindow;
class QTextStream;
template<class> class QPtrList;

/*
** -- Local Classes --
*/

class editor;
class constant;

/*
** -- Qt Includes --
*/

#include <qcanvas.h>
#include <qstring.h>
#include <qlineedit.h>
#include <qpushbutton.h>

/*
** -- Local Includes --
*/

#include <types.h>
#include <file_class.h>

class object: public QObject
{
  Q_OBJECT

 public:
  object(const int, const int, const int, QCanvas *, const char *,
	 QPtrList<file_class> * = NULL, const int = -1,
	 const int = 0, const int = 0);
  ~object();
  int getX(void);
  int getY(void);
  int getId(void);
  int getType(void);
  bool isWritten(void);
  char *getTypeName(void);
  void show(void);
  void update(const QString &);
  void setValue(const int);
  void setParent(QWidget *, const QPoint);
  void setWritten(const bool);
  void writeToFile(QTextStream &, QString &, const int);
  void setConstUpdateFunc(void (constant::*)(void));
  editor *getEditor(void);
  QPoint pos(void);
  QCanvas *getParentCanvas(void);
  QString getName(void);
  QPtrList<QCanvasEllipse> *getDots(void);

 protected:
  int x;
  int y;
  int type;
  int cvalue;
  int obj_id;
  bool written;
  char type_name[32];
  void (constant::*constUpdatePtr)(void);
  editor *edit;
  QCanvas *edit_canvas;
  QCanvas *parent_canvas;
  QString item_name;
  QWidget *dialog;
  QPtrList<QCanvasEllipse> *dots;
  QLineEdit *condition;
  QLineEdit *textfield;
  QCanvasView *view;
  QPushButton *button;
  QPushButton *cancel_button;

 protected:
  void updateLabel(void);

 private slots:
  void ok_cb(void);
  void enlarge(void);
  void cancel_cb(void);
  void setCondition(const QString &);
};

#endif
