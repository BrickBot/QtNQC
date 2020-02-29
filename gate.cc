#include <gate.h>
#include <editor.h>
#include <qtnqc_main.h>

/*
** -- gate() --
*/

gate::gate(const int x0, const int y0, const char *symbol_arg,
	   QCanvas *canvas_arg, const int type_arg,
	   const char *type_name_arg, const int obj_arg_id):
  object(type_arg, x0, y0, canvas_arg, type_name_arg, NULL, obj_arg_id)
{
  /*
  ** Draw a gate.
  */

  int i = 0;
  bool isYellow = false;
  QPointArray pa(3);

  canvas = canvas_arg;
  x = x0;
  y = y0;
  p = NULL;
  text = NULL;
  conn1 = conn1 = NULL;
  dot1 = dot2 = dot3 = NULL;
  (void) memset(symbol, '\0', sizeof(symbol));
  (void) snprintf(symbol, sizeof(symbol), "%s", symbol_arg);

  for(i = 0; i < 3; i++)
    lines[i] = NULL;

  for(i = 0; i < 3; i++)
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
  pa[1] = QPoint(x, y + 50);
  pa[2] = QPoint(x + 50, y + 25);
  p->setZ(0);
  p->setPoints(pa);

  if(strcmp(symbol, "+") == 0 ||
     strcmp(symbol, "-") == 0 ||
     strcmp(symbol, "*") == 0 ||
     strcmp(symbol, "/") == 0 ||
     strcmp(symbol, "-x") == 0 ||
     strcmp(symbol, "~") == 0 ||
     strcmp(symbol, "%") == 0 ||
     strcmp(symbol, "&") == 0 ||
     strcmp(symbol, "|") == 0 ||
     strcmp(symbol, "++") == 0 ||
     strcmp(symbol, "--") == 0 ||
     strcmp(symbol, "<<") == 0 ||
     strcmp(symbol, ">>") == 0 ||
     strcmp(symbol, "[]") == 0 ||
     strcmp(symbol, "-?") == 0 ||
     strcmp(symbol, "^") == 0)
    {
      isYellow = true;
      p->setBrush(Qt::yellow);
    }
  else
    p->setBrush(QColor("purple"));

  p->setZ(0);
  p->show();

  if((text = new QCanvasText(symbol, canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if(isYellow)
    text->setColor(Qt::black);
  else
    text->setColor(Qt::yellow);

  text->move(x + 5, y + 15);
  text->setZ(1);
  text->show();
  lines[0]->setPoints(x, y, x + 50, y + 25);
  lines[0]->show();
  lines[1]->setPoints(x + 50, y + 25, x, y + 50);
  lines[1]->show();
  lines[2]->setPoints(x, y, x, y + 50);
  lines[2]->show();

  if(type == UNARY_MINUS_GATE ||
     type == BITWISE_NEGATION_GATE ||
     type == INCREMENT_GATE ||
     type == DECREMENT_GATE ||
     type == ABSOLUTE_GATE ||
     type == SIGN_OF_GATE ||
     type == NOT_GATE ||
     type == LESS_THAN_ZERO_GATE ||
     type == GREATER_THAN_ZERO_GATE ||
     type == GREATER_THAN_ONE_GATE)
    {
      if((dot1 = new QCanvasEllipse(5, 5, 0, 360 * 16, canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      dot1->setZ(2);
      dot1->setBrush(Qt::red);
      dot1->move(x, y + 25);
      dot1->show();
      dots->append(dot1);
    }
  else
    {
      dot1 = new QCanvasEllipse(5, 5, 0, 360 * 16, canvas);
      dot2 = new QCanvasEllipse(5, 5, 0, 360 * 16, canvas);

      if(dot1 == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      if(dot2 == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      dot1->setZ(2);
      dot1->setBrush(Qt::red);
      dot1->move(x, y + 15);
      dot1->show();
      dots->append(dot1);
      dot2->setZ(2);
      dot2->setBrush(Qt::red);
      dot2->move(x, y + 35);
      dot2->show();
      dots->append(dot2);
    }

  if((dot3 = new QCanvasEllipse(5, 5, 0, 360 * 16, canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  dot3->setZ(2);
  dot3->setBrush(Qt::red);
  dot3->move(x + 50, y + 25);
  dot3->show();
  dots->append(dot3);
}

/*
** -- ~gate() --
*/

gate::~gate()
{
  int i = 0;

  for(i = 0; i < 3; i++)
    {
      delete lines[i];
      lines[i] = NULL;
    }

  delete p;
  delete text;
  delete dot1;
  delete dot2;
  delete dot3;
  dot1 = dot2 = dot3 = NULL;
  p = NULL;
  text = NULL;
}

/*
** -- getPolygon() --
*/

QCanvasPolygon *gate::getPolygon(void)
{
  return p;
}

/*
** -- move() --
*/

void gate::move(const QPoint &point, const bool subtract)
{
  QPointArray pa(3);

  if(subtract)
    {
      x = point.x() - 15;
      y = point.y() - 25;
    }

  pa[0] = QPoint(x, y);
  pa[1] = QPoint(x, y + 50);
  pa[2] = QPoint(x + 50, y + 25);

  if(p != NULL)
    p->setPoints(pa);

  if(text != NULL)
    text->move(x + 5, y + 15);

  if(lines[0] != NULL)
    lines[0]->setPoints(x, y, x + 50, y + 25);

  if(lines[1] != NULL)
    lines[1]->setPoints(x + 50, y + 25, x, y + 50);

  if(lines[2] != NULL)
    lines[2]->setPoints(x, y, x, y + 50);

  if(dot1 != NULL && dot2 == NULL)
    dot1->move(x, y + 25);
  else if(dot1 != NULL)
    dot1->move(x, y + 15);

  if(dot2 != NULL)
    dot2->move(x, y + 35);

  if(dot3 != NULL)
    dot3->move(x + 50, y + 25);
}

/*
** -- move() --
*/

bool gate::move(const int direction, const int distance)
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
** -- getSymbol() --
*/

char *gate::getSymbol(void)
{
  return symbol;
}
