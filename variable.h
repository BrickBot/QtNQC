#ifndef _VARIABLE_H_
#define _VARIABLE_H_

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

class variable: public object
{
 public:
  variable(const int, const int, QCanvas *, const int,
	   const QValueList<QString> &, editor *, const bool, const int,
	   const QString &);
  ~variable();
  QCanvasPolygon *getPolygon(void);
  bool move(const int, const int);
  bool isGlobal(void);
  void move(const QPoint &, const bool = true);
  void update(const QString &);
  void setAsGlobal(void);

 private:
  editor *edit_container;
  QCanvas *canvas;
  QCanvasLine *lines[4];
  QCanvasText *name;
  QCanvasText *text;
  QCanvasEllipse *dot1;
  QCanvasEllipse *dot2;
  QCanvasPolygon *p;
};

#endif
