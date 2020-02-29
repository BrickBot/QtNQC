#ifndef _TASK_H_
#define _TASK_H_

/*
** -- Qt Classes --
*/

class QPoint;
class QCanvas;
class QCanvasLine;
class QCanvasText;
class QCanvasEllipse;
class QCanvasPolygon;

/*
** -- Local Includes --
*/

#include <types.h>
#include <object.h>
#include <file_class.h>

class task: public object
{
 public:
  task(const int, const int, QCanvas *, const int,
       QPtrList<file_class> *, const int = -1, const QString & = "",
       const int = 0, const int = 0);
  ~task();
  bool move(const int, const int);
  void move(const QPoint &, const bool = true);
  void update(const QString &);
  QCanvasPolygon *getPolygon(void);

 private:
  QCanvas *canvas;
  QCanvasLine *lines[4];
  QCanvasText *name;
  QCanvasText *text;
  QCanvasEllipse *dot1;
  QCanvasEllipse *dot2;
  QCanvasPolygon *p;
};

#endif
