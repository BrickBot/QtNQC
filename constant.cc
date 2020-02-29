#include <editor.h>
#include <constant.h>
#include <qtnqc_main.h>

/*
** -- constant() --
*/

constant::constant(const int x0, const int y0, QCanvas *canvas_arg,
		   const int type, editor *edit_arg, int const obj_arg_id,
		   int const value_arg):
  object(type, x0, y0, canvas_arg, "Constant", NULL, obj_arg_id)
{
  /*
  ** Draw a constant.
  */

  int i = 0;
  char tmpstr[16];
  QPointArray pa(4);

  cvalue = value_arg;
  (void) snprintf(tmpstr, sizeof(tmpstr), "%d", cvalue);
  edit_container = edit_arg;
  setConstUpdateFunc(&constant::updateValue);
  canvas = canvas_arg;
  x = x0;
  y = y0;
  p = NULL;
  text = NULL;
  value = NULL;

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

  if((value = new QCanvasText(tmpstr, canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  value->setColor(Qt::blue);
  value->move(x, y - 20);
  value->setZ(1);
  value->show();

  if((text = new QCanvasText("C", canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  text->setColor(Qt::blue);
  text->move(x + 20, y + 15);
  text->setZ(1);
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
** -- ~constant() --
*/

constant::~constant()
{
  int i = 0;

  for(i = 0; i < 4; i++)
    {
      delete lines[i];
      lines[i] = NULL;
    }

  delete p;
  delete text;
  delete value;
  delete dot1;
  delete dot2;
  dot1 = dot2 = NULL;
  p = NULL;
  value = text = NULL;
}

/*
** -- getPolygon() --
*/

QCanvasPolygon *constant::getPolygon(void)
{
  return p;
}

/*
** -- move() --
*/

void constant::move(const QPoint &point, const bool subtract)
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
  value->move(x, y - 20);
  lines[0]->setPoints(x, y, x + 50, y);
  lines[1]->setPoints(x + 50, y, x + 50, y + 50);
  lines[2]->setPoints(x + 50, y + 50, x, y + 50);
  lines[3]->setPoints(x, y + 50, x, y);
  dot1->move(x, y + 25);
  dot2->move(x + 50, y + 25);
  updateLabel();
}

/*
** -- move() --
*/

bool constant::move(const int direction, const int distance)
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
** -- getValue() --
*/

int constant::getValue(void)
{
  return cvalue;
}

/*
** -- updateValue() --
*/

void constant::updateValue(void)
{
  char buffer[6];

  (void) snprintf(buffer, sizeof(buffer), "%d", cvalue);
  value->setText(buffer);
  canvas->update();

  if(edit_container != NULL)
    edit_container->updateLister();
}
