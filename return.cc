#include <return.h>
#include <editor.h>
#include <qtnqc_main.h>

/*
** -- return_obj() --
*/

return_obj::return_obj(const int x0, const int y0,
		       QCanvas *canvas_arg, const int obj_arg_id):
  object(RETURN, x0, y0, canvas_arg, "", NULL, obj_arg_id)
{
  /*
  ** Draw a return_obj.
  */

  int i = 0;
  QPointArray pa1(4);

  canvas = canvas_arg;
  x = x0;
  y = y0;
  p = NULL;
  text = NULL;

  for(i = 0; i < 4; i++)
    lines[i] = NULL;

  for(i = 0; i < 4; i++)
    {
      lines[i] = new QCanvasLine(canvas);

      if(lines[i] == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      lines[i]->setPen(Qt::black);
      lines[i]->setZ(5);
    }

  if((p = new QCanvasPolygon(canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  pa1[0] = QPoint(x, y);
  pa1[1] = QPoint(x + 50, y);
  pa1[2] = QPoint(x + 50, y + 50);
  pa1[3] = QPoint(x, y + 50);
  p->setPoints(pa1);
  p->setBrush(QColor("tan"));
  p->setZ(1);
  p->show();

  if((text = new QCanvasText("R", canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  text->setColor(QColor("darkBlue"));
  text->move(x + 20, y + 15);
  text->setZ(10);
  text->show();
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

  dot1->setZ(5);
  dot1->setBrush(Qt::red);
  dot1->move(x, y + 25);
  dot1->show();
  dots->append(dot1);

  if((dot2 = new QCanvasEllipse(5, 5, 0, 360 * 16, canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  dot2->setZ(5);
  dot2->setBrush(Qt::red);
  dot2->move(x + 50, y + 25);
  dot2->show();
  dots->append(dot2);
}

/*
** -- ~return_obj() --
*/

return_obj::~return_obj()
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
  dot1 = dot2  = NULL;
  p = NULL;
  text = NULL;
}

/*
** -- getPolygon() --
*/

QCanvasPolygon *return_obj::getPolygon(void)
{
  return p;
}

/*
** -- move() --
*/

void return_obj::move(const QPoint &point, const bool subtract)
{
  QPointArray pa1(4);

  if(subtract)
    {
      x = point.x() - 25;
      y = point.y() - 25;
    }

  pa1[0] = QPoint(x, y);
  pa1[1] = QPoint(x, y + 50);
  pa1[2] = QPoint(x + 50, y + 50);
  pa1[3] = QPoint(x + 50, y);

  p->setPoints(pa1);
  text->move(x + 20, y + 15);
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

bool return_obj::move(const int direction, const int distance)
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
