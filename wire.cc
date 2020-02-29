#include <wire.h>
#include <editor.h>

/*
** -- wire() --
*/

template class QPtrList<QCanvasLine>;

wire::wire(QCanvas *canvas_arg, const QPoint &a, const QPoint &b):
  object(WIRE, 0, 0, canvas_arg, (const char *) NULL)
{
  int i = 0;
  int how_many = 5;
  QPoint points[6];
  QCanvasLine *line = NULL;

  lines = NULL;
  canvas = canvas_arg;
  pt1 = a;
  pt2 = b;

  if((lines = new QPtrList<QCanvasLine>()) == NULL)
    (void) fprintf(stderr, "Memory allocation failure in file %s, line %d.\n",
		   __FILE__, __LINE__);
  else
    {
      lines->setAutoDelete(true);

      points[0].setX(a.x());
      points[0].setY(a.y());

      if(canvas->collisions(QPoint(a.x() - 10, a.y())).size() > 0)
	points[1].setX(a.x() + 10);
      else
	points[1].setX(a.x() - 10);

      points[1].setY(a.y());
      points[5].setX(b.x());
      points[5].setY(b.y());

      if(canvas->collisions(QPoint(b.x() - 10, b.y())).size() > 0)
	points[4].setX(b.x() + 10);
      else
	points[4].setX(b.x() - 10);

      points[4].setY(b.y());
      points[3].setX(points[4].x());

      if(a.y() >= b.y())
	{
	  if(a.y() - (a.y() - b.y()) / 2 <= 55)
	    points[3].setY(a.y() + (a.y() - b.y()) / 2);
	  else
	    points[3].setY(a.y() - (a.y() - b.y()) / 2);
	}
      else
	{
	  if(b.y() - (b.y() - a.y()) / 2 <= 55)
	    points[3].setY(b.y() + (b.y() - a.y()) / 2);
	  else
	    points[3].setY(b.y() - (b.y() - a.y()) / 2);
	}

      if(QABS(a.y() - b.y()) <= 50)
	if(a.y() - 50 <= 10)
	  points[3].setY(a.y() + 50);
	else
	  points[3].setY(a.y() - 50);

      points[2].setX(points[1].x());
      points[2].setY(points[3].y());

      for(i = 0; i < how_many; i++)
	if((line = new QCanvasLine(canvas)) != NULL)
	  {
	    line->setPen(QPen(Qt::blue, 2));
	    line->setPoints(points[i].x(), points[i].y(),
			    points[i + 1].x(), points[i + 1].y());
	    line->setZ(-1);
	    line->show();
	    canvas->update();
	    lines->append(line);
	  }
	else
	  (void) fprintf(stderr, "Memory allocation failure in file %s, "
			 "line %d.\n", __FILE__, __LINE__);
    }
}

/*
** -- ~wire() --
*/

wire::~wire()
{
  if(lines != NULL)
    lines->clear();

  delete lines;
  lines = NULL;
}

/*
** -- exists() --
*/

bool wire::exists(const QPoint &a, const QPoint &b)
{
  unsigned int i = 0;

  if(lines != NULL)
    {
      for(i = 0; i < (unsigned int) lines->count(); i++)
	if((lines->at(i)->startPoint() == a &&
	    lines->at(i)->endPoint() == b) ||
	   (lines->at(i)->startPoint() == b &&
	    lines->at(i)->endPoint() == a))
	  return true;

      return false;
    }
  else
    return false;
}
