/*
** -- Local Includes --
*/

#include <gate.h>
#include <loop.h>
#include <types.h>
#include <dragobject.h>

/*
** -- dragobject() --
*/

dragobject::dragobject(void *o, QWidget *parent,
		       const char *name): QStoredDrag("secret/magic",
						      parent, name)
{
  if(o != NULL)
    ;
}

/*
** -- canDecode() --
*/

bool dragobject::canDecode(QDragMoveEvent *e)
{
  if(e != NULL)
    return e->provides("secret/magic");
  else
    return false;
}

/*
** -- decode() --
*/

bool dragobject::decode(QDropEvent *e, const int type)
{
  if(type == GATE || type == LOOP || type == CONSTANT ||
     type == VARIABLE || type == TASK || type == FUNCTION ||
     type == BREAK || type == CONTINUE || type == RETURN)
    {
      if(e != NULL)
	{
	  e->accept();
	  return true;
	}
      else
	return false;
    }
  else
    {
      if(e != NULL)
	e->ignore();

      return false;
    }
}

/*
** -- ~dragobject() --
*/

dragobject::~dragobject()
{
}
