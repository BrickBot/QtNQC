#ifndef _LOOP_H_
#define _LOOP_H_

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

class loop: public object
{
 public:
  loop(const int, const int, const char *, const char *, QCanvas *,
       const int, const char *, QPtrList<file_class> *, const int = -1,
       const QString & = "", const int = 0, const int = 0);
  ~loop();
  QCanvasPolygon *getPolygon(void);
  bool move(const int, const int);
  void move(const QPoint &, const bool = true);
  void setCondition(const QString &);
  QString getCondition(void);

 private:
  QCanvas *canvas;
  QString condition_str;
  QCanvasLine *lines[4];
  QCanvasText *text1;
  QCanvasText *text2;
  QCanvasEllipse *dot1;
  QCanvasEllipse *dot2;
  QCanvasPolygon *p;
};

#endif
