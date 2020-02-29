#ifndef _RETURN_H_
#define _RETURN_H_

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

class return_obj: public object
{
 public:
  return_obj(const int, const int, QCanvas *, const int = -1);
  ~return_obj();
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
