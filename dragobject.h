#ifndef _DRAGOBJECT_H_
#define _DRAGOBJECT_H_

/*
** -- Qt Includes --
*/

#include <qdragobject.h>

class dragobject: public QStoredDrag
{
 public:
  dragobject(void *, QWidget * = NULL, const char * = NULL);
  ~dragobject();

  static bool decode(QDropEvent *, const int);
  static bool canDecode(QDragMoveEvent *);
};

#endif
