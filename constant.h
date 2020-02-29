#ifndef _CONSTANT_H_
#define _CONSTANT_H_

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

class constant: public object
{
 public:
  constant(const int, const int, QCanvas *, const int, editor *,
	   const int = -1, const int = 0);
  ~constant();
  QCanvasPolygon *getPolygon(void);
  int getValue(void);
  bool move(const int, const int);
  void move(const QPoint &, const bool = true);

 private:
  void updateValue(void);
  editor *edit_container;
  QCanvas *canvas;
  QCanvasLine *lines[4];
  QCanvasText *text;
  QCanvasText *value;
  QCanvasEllipse *dot1;
  QCanvasEllipse *dot2;
  QCanvasPolygon *p;
};

#endif
