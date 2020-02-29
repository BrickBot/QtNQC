#ifndef _CONTINUE_H_
#define _CONTINUE_H_

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

class continue_obj: public object
{
 public:
  continue_obj(const int, const int, QCanvas *, const int = -1);
  ~continue_obj();
  QCanvasPolygon *getPolygon(void);
  bool move(const int, const int);
  void move(const QPoint &, const bool = true);

 private:
  object *conn1;
  object *conn2;
  QCanvas *canvas;
  QCanvasLine *lines[4];
  QCanvasText *text;
  QCanvasEllipse *dot1;
  QCanvasEllipse *dot2;
  QCanvasPolygon *p;
};

#endif
