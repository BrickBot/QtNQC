#ifndef _GATE_H_
#define _GATE_H_

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

class gate: public object
{
 public:
  gate(const int, const int, const char *, QCanvas *, const int,
       const char *, const int = -1);
  ~gate();
  QCanvasPolygon *getPolygon(void);
  bool move(const int, const int);
  char *getSymbol(void);
  void move(const QPoint &, const bool = true);

 private:
  char symbol[16];
  object *conn1;
  object *conn2;
  QCanvas *canvas;
  QCanvasLine *lines[3];
  QCanvasText *text;
  QCanvasEllipse *dot1;
  QCanvasEllipse *dot2;
  QCanvasEllipse *dot3;
  QCanvasPolygon *p;
};

#endif
