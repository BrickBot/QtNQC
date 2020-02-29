#include <task.h>
#include <editor.h>
#include <qtnqc_main.h>

extern qtnqc_main *qmain;

/*
** -- task() --
*/

task::task(const int x0, const int y0, QCanvas *canvas_arg,
	   const int type_arg, QPtrList<file_class> *list,
	   const int obj_id_arg, const QString &name_arg,
	   const int cwidth, const int cheight):
  object(type_arg, x0, y0, canvas_arg, "Task", list, obj_id_arg,
	 cwidth, cheight)
{
  /*
  ** Draw a task.
  */

  int i = 0;
  char buffer[16];
  QString tmp = "task";
  QPointArray pa(4);

  item_name = name_arg;
  canvas = canvas_arg;
  x = x0;
  y = y0;
  p = NULL;
  text = NULL;
  name = NULL;

  if(item_name.isEmpty())
    {
      for(i = 0; i < MAX_TASKS; i++)
	{
	  (void) snprintf(buffer, sizeof(buffer), "%d", i);

	  if(!qmain->getUsedTaskNames().contains(tmp.append(buffer)))
	    {
	      item_name = tmp;
	      qmain->getUsedTaskNames().append(tmp);
	      break;
	    }
	  else
	    tmp = "task";
	}
    }
  else
    qmain->getUsedTaskNames().append(item_name);

  for(i = 0; i < 4; i++)
    lines[i] = NULL;

  for(i = 0; i < 4; i++)
    {
      lines[i] = new QCanvasLine(canvas);

      if(lines[i] == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      lines[i]->setPen(Qt::black);
      lines[i]->setZ(1);
    }

  if((p = new QCanvasPolygon(canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  pa[0] = QPoint(x, y);
  pa[1] = QPoint(x + 50, y);
  pa[2] = QPoint(x + 50, y + 50);
  pa[3] = QPoint(x, y + 50);
  p->setPoints(pa);
  p->setBrush(Qt::green);
  p->setZ(0);
  p->show();

  if((text = new QCanvasText("t()", canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  text->setColor(Qt::blue);
  text->move(x + 20, y + 15);
  text->setZ(1);
  text->show();

  if((name = new QCanvasText(item_name, canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  name->setColor(Qt::blue);
  name->move(x, y - 20);
  name->setZ(1);
  name->show();
  lines[0]->setPoints(x, y, x + 50, y);
  lines[0]->show();
  lines[1]->setPoints(x + 50, y, x + 50, y + 50);
  lines[1]->show();
  lines[2]->setPoints(x + 50, y + 50, x, y + 50);
  lines[2]->show();
  lines[3]->setPoints(x, y + 50, x, y);
  lines[3]->show();

  if((dot1 = new QCanvasEllipse(5, 5, 0, 360 * 16, canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  dot1->setZ(2);
  dot1->setBrush(Qt::red);
  dot1->move(x, y + 25);
  dot1->show();
  dots->append(dot1);

  if((dot2 = new QCanvasEllipse(5, 5, 0, 360 * 16, canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  dot2->setZ(2);
  dot2->setBrush(Qt::red);
  dot2->move(x + 50, y + 25);
  dot2->show();
  dots->append(dot2);
}

/*
** -- ~task() --
*/

task::~task()
{
  int i = 0;

  for(i = 0; i < 4; i++)
    {
      delete lines[i];
      lines[i] = NULL;
    }

  delete p;
  delete text;
  delete name;
  delete dot1;
  delete dot2;
  dot1 = dot2 = NULL;
  p = NULL;
  text = NULL;
  name = NULL;
  qmain->getUsedTaskNames().remove(item_name);
}

/*
** -- getPolygon() --
*/

QCanvasPolygon *task::getPolygon(void)
{
  return p;
}

/*
** -- move() --
*/

void task::move(const QPoint &point, const bool subtract)
{
  QPointArray pa(4);

  if(subtract)
    {
      x = point.x() - 25;
      y = point.y() - 25;
    }

  pa[0] = QPoint(x, y);
  pa[1] = QPoint(x, y + 50);
  pa[2] = QPoint(x + 50, y + 50);
  pa[3] = QPoint(x + 50, y);

  if(p != NULL)
    p->setPoints(pa);

  if(text != NULL)
    text->move(x + 20, y + 15);

  if(name != NULL)
    name->move(x, y - 20);

  if(lines[0] != NULL)
    lines[0]->setPoints(x, y, x + 50, y);

  if(lines[1] != NULL)
    lines[1]->setPoints(x + 50, y, x + 50, y + 50);

  if(lines[2] != NULL)
    lines[2]->setPoints(x + 50, y + 50, x, y + 50);

  if(lines[3] != NULL)
    lines[3]->setPoints(x, y + 50, x, y);

  if(dot1 != NULL)
    dot1->move(x, y + 25);

  if(dot2 != NULL)
    dot2->move(x + 50, y + 25);

  updateLabel();
}

/*
** -- move() --
*/

bool task::move(const int direction, const int distance)
{
  QPoint pt;

  if(direction == editor::MOVE_UP)
    if(!canvas->onCanvas(x, y - distance))
      return false;

  if(direction == editor::MOVE_DOWN)
    if(!canvas->onCanvas(x, y + distance))
      return false;

  if(direction == editor::MOVE_LEFT)
    if(!canvas->onCanvas(x - distance, y))
      return false;

  if(direction == editor::MOVE_RIGHT)
    if(!canvas->onCanvas(x + distance, y))
      return false;

  if(direction == editor::MOVE_UP)
    y -= distance;
  else if(direction == editor::MOVE_DOWN)
    y += distance;
  else if(direction == editor::MOVE_LEFT)
    x -= distance;
  else
    x += distance;

  pt.setX(x);
  pt.setY(y);
  move(pt, false);
  return true;
}

/*
** -- update() --
*/

void task::update(const QString &item_name_arg)
{
  item_name = item_name_arg;

  if(name != NULL)
    name->setText(item_name);

  canvas->update();
  updateLabel();
}
