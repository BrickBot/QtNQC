#ifndef _WIRE_H_
#define _WIRE_H_

/*
** -- Qt Classes --
*/

class QCanvas;
class QCanvasLine;
class QCanvasText;
class QCanvasEllipse;
class QCanvasPolygon;
template<class> class QPtrList;

/*
** -- Qt Includes --
*/

#include <qpoint.h>

/*
** -- Local Includes --
*/

#include <types.h>
#include <object.h>

class wire: public object
{
 public:
  wire(QCanvas *, const QPoint &, const QPoint &);
  ~wire();
  bool exists(const QPoint &, const QPoint &);

 private:
  QPoint pt1;
  QPoint pt2;
  QCanvas *canvas;
  QPtrList<QCanvasLine> *lines;
};

#endif
