#include <types.h>
#include <qtnqc_main.h>
#include <tools_window.h>

extern qtnqc_main *qmain;

/*
** -- tools_window() --
*/

tools_window::tools_window(QCanvas *canvas_arg, QWidget *parent_arg,
			   const char *name_arg,
			   WFlags f_arg):
  QCanvasView(canvas_arg, parent_arg, name_arg, f_arg)
{
  int i = 0;
  int x = 0;
  int y = 0;
  const char *symbols[] = {"Curs", "Wire", "Del", "DelA"};
  QCanvasText *text = NULL;
  QPointArray pa(4);
  QCanvasPolygon *p = NULL;
  QCanvasRectangle *rec = NULL;
  static const int square_s = 50;

  /*
  ** Create a window with all the functions.
  */

  parent = parent_arg;
  canvas = canvas_arg;
  canvas->setBackgroundColor(QColor("brown"));
  setMinimumSize(210, 60);

  items.append(CURSOR);
  items.append(WIRE);
  items.append(DELETE);
  items.append(DELETEALL);

  if((selection = new QCanvasRectangle(1, 1, square_s, square_s,
				       canvas)) != NULL)
    {
      selection->setPen(Qt::white);
      selection->setZ(1);
    }
  else
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  for(i = 0, x = 1, y = 1; i < (int) (sizeof(symbols) / sizeof(symbols[0]));
      i++, x += square_s)
    {
      if((rec = new QCanvasRectangle(x, y, square_s, square_s,
				     canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      rec->setPen(Qt::black);
      rec->setZ(0);
      rec->show();

      if((p = new QCanvasPolygon(canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      pa[0] = QPoint(x + 1, y + 1);
      pa[1] = QPoint(x + square_s - 1, y + 1);
      pa[2] = QPoint(x + square_s - 1, y + square_s - 1);
      pa[3] = QPoint(x + 1, y + square_s - 1);
      p->setBrush(QColor("navy"));
      p->setPoints(pa);
      p->setZ(0);
      p->show();

      if((text = new QCanvasText(symbols[i], canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      text->setColor(Qt::yellow);
      text->move(x + 5, y + 15);
      text->setZ(1);
      text->show();
    }

  type = items[0];
  selection->move(1, 1);
  selection->show();
  canvas->update();
}

/*
** -- ~tools_window() --
*/

tools_window::~tools_window()
{
  delete selection;
  selection = NULL;
  items.clear();
}

/*
** -- contentsMousePressEvent() --
*/

void tools_window::contentsMousePressEvent(QMouseEvent *e)
{
  int i = 0;
  int j = 0;
  char buffer[32];

  if(e->button() != Qt::LeftButton)
    return;

  type = -1;

  for(j = 0; j < (int) items.size() / ITEMS_COLS; j++)
    for(i = 0; i < ITEMS_COLS; i++)
      if(e->x() >= 50 * i + 1 && e->x() <= 50 * (i + 1) + 1)
	if(e->y() >= 50 * j + 1 && e->y() <= 50 * (j + 1) + 1)
	  {
	    type = items[i + ITEMS_COLS * j];
	    selection->move(50 * i, 50 * j);
	    goto done_label;
	  }

 done_label:

  if(type > -1)
    {
      QApplication::restoreOverrideCursor();

      if(type == CURSOR)
	(void) strncpy(buffer, "Cursor", sizeof(buffer) - 1);
      else if(type == WIRE)
	(void) strncpy(buffer, "Wire", sizeof(buffer) - 1);
      else if(type == DELETE)
	{
	  (void) strncpy(buffer, "Delete", sizeof(buffer) - 1);
	  QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
	}
      else if(type == DELETEALL)
	(void) strncpy(buffer, "Delete All", sizeof(buffer) - 1);
      else
	(void) strncpy(buffer, "None", sizeof(buffer) - 1);

      ((QMainWindow *) parent)->statusBar()->message(buffer);
      selection->show();
    }
  else
    {
      ((QMainWindow *) parent)->statusBar()->message("None");
      selection->hide();
    }

  canvas->update();
}

/*
** -- displayDialog() --
*/

void tools_window::displayDialog(void)
{
  parent->show();
}

/*
** -- hideDialog() --
*/

void tools_window::hideDialog(void)
{
  parent->hide();
}

/*
** -- getType() --
*/

int tools_window::getType(void)
{
  if(!selection->isVisible())
    return -1;
  else
    return type;
}

/*
** -- getParent() --
*/

QWidget *tools_window::getParent(void)
{
  return parent;
}
