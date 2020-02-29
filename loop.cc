#include <loop.h>
#include <editor.h>
#include <qtnqc_main.h>

/*
** -- loop() --
*/

loop::loop(const int x0, const int y0, const char *index_symbol,
	   const char *other_symbol, QCanvas *canvas_arg,
	   const int type_arg, const char *type_name_arg,
	   QPtrList<file_class> *list, const int obj_arg_id,
	   const QString &condition_str_arg,
	   const int cwidth, const int cheight):
  object(type_arg, x0, y0, canvas_arg, type_name_arg, list, obj_arg_id,
	 cwidth, cheight)
{
  /*
  ** Draw a loop.
  */

  int i = 0;
  QPointArray pa(4);

  canvas = canvas_arg;
  x = x0;
  y = y0;
  p = NULL;
  text1 = text2 = NULL;
  condition_str = condition_str_arg;
  condition->setText(condition_str);

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
  p->setBrush(QColor("orange"));
  p->setZ(0);
  p->show();

  if((text1 = new QCanvasText(index_symbol, canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  text1->setColor(Qt::red);
  text1->move(x + 5, y + 30);
  text1->setZ(1);
  text1->show();

  if((text2 = new QCanvasText(other_symbol, canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if(strcmp(other_symbol, "N") == 0)
    text2->setColor(Qt::blue);
  else
    text2->setColor(QColor("purple"));

  text2->move(x + 35, y);
  text2->setZ(1);
  text2->show();
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
** -- ~loop() --
*/

loop::~loop()
{
  int i = 0;

  for(i = 0; i < 4; i++)
    {
      delete lines[i];
      lines[i] = NULL;
    }

  delete p;
  delete text1;
  delete text2;
  delete dot1;
  delete dot2;
  dot1 = dot2 = NULL;
  p = NULL;
  text1 = text2 = NULL;
}

/*
** -- getPolygon() --
*/

QCanvasPolygon *loop::getPolygon(void)
{
  return p;
}

/*
** -- move() --
*/

void loop::move(const QPoint &point, const bool subtract)
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

  if(text1 != NULL)
    text1->move(x + 5, y + 30);

  if(text2 != NULL)
    text2->move(x +35, y);

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

bool loop::move(const int direction, const int distance)
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
** -- setCondition() --
*/

void loop::setCondition(const QString &str)
{
  condition_str = str;
}


/*
** -- getCondition() --
*/

QString loop::getCondition(void)
{
  return condition_str;
}
