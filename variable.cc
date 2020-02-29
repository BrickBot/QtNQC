#include <editor.h>
#include <variable.h>
#include <qtnqc_main.h>

/*
** -- variable() --
*/

variable::variable(const int x0, const int y0, QCanvas *canvas_arg,
		   const int type_arg, const QValueList<QString> &list,
		   editor *edit_arg, const bool is_global_arg,
		   const int obj_arg_id, const QString &copy_name):
  object(type_arg, x0, y0, canvas_arg, "Variable", NULL, obj_arg_id)
{
  /*
  ** Draw a variable.
  */

  int i = 0;
  char buffer[16];
  QString tmp = "x";
  QPointArray pa(4);

  edit_container = edit_arg;
  item_name = "x";
  canvas = canvas_arg;
  x = x0;
  y = y0;
  name = NULL;
  p = NULL;
  text = NULL;

  if(copy_name.isEmpty())
    {
      for(i = 0; i < 9999; i++)
	{
	  (void) snprintf(buffer, sizeof(buffer), "%d", i);

	  if(!list.contains(tmp.append(buffer)))
	    {
	      item_name = tmp;
	      break;
	    }
	  else
	    tmp = "x";
	}
    }
  else
    item_name = copy_name;

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

  if(is_global_arg)
    p->setBrush(Qt::yellow);
  else
    p->setBrush(Qt::green);

  p->setZ(0);
  p->show();

  if(is_global_arg)
    (void) snprintf(buffer, sizeof(buffer), "%s", "G");
  else
    (void) snprintf(buffer, sizeof(buffer), "%s", "x");

  if((text = new QCanvasText(buffer, canvas)) == NULL)
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
** -- ~variable() --
*/

variable::~variable()
{
  int i = 0;

  for(i = 0; i < 4; i++)
    {
      delete lines[i];
      lines[i] = NULL;
    }

  delete p;
  delete text;
  delete dot1;
  delete dot2;
  delete name;
  dot1 = dot2 = NULL;
  p = NULL;
  name = text = NULL;
}

/*
** -- getPolygon() --
*/

QCanvasPolygon *variable::getPolygon(void)
{
  return p;
}

/*
** -- move() --
*/

void variable::move(const QPoint &point, const bool subtract)
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

  p->setPoints(pa);
  text->move(x + 20, y + 15);
  name->move(x, y - 20);
  lines[0]->setPoints(x, y, x + 50, y);
  lines[1]->setPoints(x + 50, y, x + 50, y + 50);
  lines[2]->setPoints(x + 50, y + 50, x, y + 50);
  lines[3]->setPoints(x, y + 50, x, y);
  dot1->move(x, y + 25);
  dot2->move(x + 50, y + 25);
}

/*
** -- move() --
*/

bool variable::move(const int direction, const int distance)
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

void variable::update(const QString &item_name_arg)
{
  item_name = item_name_arg;
  name->setText(item_name);
  canvas->update();
  updateLabel();

  if(edit_container != NULL)
    edit_container->updateLister();
}

/*
** -- isGlobal() --
*/

bool variable::isGlobal(void)
{
  return text->text().contains("G");
}

/*
** -- setAsGlobal() --
*/

void variable::setAsGlobal(void)
{
  if(text->text().contains("G"))
    {
      text->setText("x");
      p->setBrush(Qt::green);
    }
  else
    {
      text->setText("G");
      p->setBrush(Qt::yellow);
    }

  canvas->update();
}
