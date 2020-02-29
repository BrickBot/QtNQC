#include <types.h>
#include <qtnqc_main.h>
#include <function_window.h>

/*
** -- function_window() --
*/

function_window::function_window(QCanvas *canvas_arg, QWidget *parent_arg,
				 const char *name_arg, WFlags f_arg):
  QCanvasView(canvas_arg, parent_arg, name_arg, f_arg)
{
  int i = 0;
  int j = 0;
  int x = 0;
  int y = 0;
  const char *vars_symbols[] = {"C", "x", "t()", "f()"};
  const char *other1_symbols[] = {"s()", "B", "GO", "R"};
  const char *boolean_symbols[] = {"!", "&&", "||", "==", ">", "<", ">=", "<=",
				   "!=", "<0", ">0", ">1"};
  const char *arithmetic_symbols[] = {"+", "-", "*", "/", "-x", "~", "%", "^",
				      "&", "|", "++", "--", "<<", ">>",
				      "[]", "-?"};
  QCanvasText *text = NULL;
  QCanvasLine *lines[4];
  QPointArray pa(3);
  QPointArray fpa1(4); /* Loops */
  QPointArray fpa2(8); /* Break */
  QCanvasPolygon *p = NULL;
  QCanvasRectangle *rec = NULL;

  items.append(ADD_GATE);
  descriptions.append("Add");
  items.append(SUBTRACT_GATE);
  descriptions.append("Subtract");
  items.append(MULTIPLY_GATE);
  descriptions.append("Multiply");
  items.append(DIVIDE_GATE);
  descriptions.append("Divide");
  items.append(UNARY_MINUS_GATE);
  descriptions.append("Unary Minus");
  items.append(BITWISE_NEGATION_GATE);
  descriptions.append("Bitwise Negation");
  items.append(MODULO_GATE);
  descriptions.append("Modulo");
  items.append(BITWISE_XOR_GATE);
  descriptions.append("Bitwise Xor");
  items.append(BITWISE_AND_GATE);
  descriptions.append("Bitwise And");
  items.append(BITWISE_OR_GATE);
  descriptions.append("Bitwise Or ");
  items.append(INCREMENT_GATE);
  descriptions.append("Increment");
  items.append(DECREMENT_GATE);
  descriptions.append("Decrement");
  items.append(LEFT_SHIFT_GATE);
  descriptions.append("Left Shift");
  items.append(RIGHT_SHIFT_GATE);
  descriptions.append("Right Shift");
  items.append(ABSOLUTE_GATE);
  descriptions.append("Absolute Value");
  items.append(SIGN_OF_GATE);
  descriptions.append("Sign Of");
  items.append(NOT_GATE);
  descriptions.append("Not");
  items.append(AND_GATE);
  descriptions.append("And");
  items.append(OR_GATE);
  descriptions.append("Or");
  items.append(EQUALS_GATE);
  descriptions.append("Equals");
  items.append(GREATER_THAN_GATE);
  descriptions.append("Greater Than");
  items.append(LESS_THAN_GATE);
  descriptions.append("Less Than");
  items.append(GREATER_THAN_OR_EQUAL_TO_GATE);
  descriptions.append("GT or ET");
  items.append(LESS_THAN_OR_EQUAL_TO_GATE);
  descriptions.append("LT or ET");
  items.append(NOT_EQUAL_TO_GATE);
  descriptions.append("Not Equal To");
  items.append(LESS_THAN_ZERO_GATE);
  descriptions.append("Less Than Zero");
  items.append(GREATER_THAN_ZERO_GATE);
  descriptions.append("Greater Than Zero");
  items.append(GREATER_THAN_ONE_GATE);
  descriptions.append("Greater Than One");
  items.append(FOR_LOOP);
  descriptions.append("For Loop");
  items.append(REPEAT_LOOP);
  descriptions.append("Repeat Loop");
  items.append(WHILE_LOOP);
  descriptions.append("While Loop");
  items.append(DO_WHILE_LOOP);
  descriptions.append("Do While Loop");
  items.append(SUBROUTINE);
  descriptions.append("Subroutine");
  items.append(BREAK);
  descriptions.append("Break");
  items.append(CONTINUE);
  descriptions.append("Continue");
  items.append(RETURN);
  descriptions.append("Return");
  items.append(CONSTANT);
  descriptions.append("Constant");
  items.append(VARIABLE);
  descriptions.append("Variable");
  items.append(TASK);
  descriptions.append("Task");
  items.append(FUNCTION);
  descriptions.append("Function");

  /*
  ** Create a window with all the functions.
  */

  parent = parent_arg;
  canvas = canvas_arg;
  canvas->setBackgroundColor(QColor("brown"));

  /*
  ** setMinimumSize(210, 510);
  */

  if((selection = new QCanvasRectangle(1, 1, square_s, square_s,
				       canvas)) != NULL)
    {
      selection->setPen(QColor("azure"));
      selection->setZ(1);
    }
  else
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  /*
  ** Draw the arithmetic functions.
  */

  for(i = 0, x = 1, y = 1;
      i < (int) (sizeof(arithmetic_symbols) / sizeof(arithmetic_symbols[0]));
      i++, x += square_s)
    {
      if(i % 4 == 0 && i != 0)
	{
	  x = 1;
	  y += square_s;
	}

      if((rec = new QCanvasRectangle(x, y, square_s, square_s,
				     canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      rec->setPen(Qt::black);
      rec->setZ(0);
      rec->show();
    }

  for(i = 0, j = 0, x = 5, y = 5;
      i < (int) (sizeof(arithmetic_symbols) / sizeof(arithmetic_symbols[0]));
      i++, x += square_s)
    {
      for(j = 0; j < 3; j++)
	{
	  lines[j] = new QCanvasLine(canvas);

	  if(lines[j] == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  lines[j]->setPen(Qt::black);
	  lines[j]->setZ(1);
	}

      if(i % 4 == 0 && i != 0)
	{
	  x = 5;
	  y += square_s;
	}

      if((p = new QCanvasPolygon(canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      pa[0] = QPoint(x, y);
      pa[1] = QPoint(x, y + 40);
      pa[2] = QPoint(x + 40, y + 20);
      p->setBrush(Qt::yellow);
      p->setPoints(pa);
      p->setZ(0);
      p->show();

      if((text = new QCanvasText(arithmetic_symbols[i],
				 canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      text->setColor(Qt::black);
      text->move(x + 5, y + 10);
      text->setZ(1);
      text->show();
      lines[0]->setPoints(x, y, x + 40, y + 20);
      lines[0]->show();
      lines[1]->setPoints(x + 40, y + 20, x, y + 40);
      lines[1]->show();
      lines[2]->setPoints(x, y, x, y + 40);
      lines[2]->show();
    }

  /*
  ** Draw boolean functions.
  */

  for(i = 0, x = 5, y += square_s;
      i < (int) (sizeof(boolean_symbols) / sizeof(boolean_symbols[0]));
      i++, x += square_s)
    {
      if(i % 4 == 0 && i != 0)
	{
	  x = 5;
	  y += square_s;
	}

      if((rec = new QCanvasRectangle(x - 4, y - 6, square_s, square_s,
				     canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      rec->setPen(Qt::black);
      rec->setZ(0);
      rec->show();

      for(j = 0; j < 3; j++)
	{
	  lines[j] = new QCanvasLine(canvas);

	  if(lines[j] == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  lines[j]->setPen(Qt::black);
	  lines[j]->setZ(1);
	}

      if((p = new QCanvasPolygon(canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      pa[0] = QPoint(x, y);
      pa[1] = QPoint(x, y + 40);
      pa[2] = QPoint(x + 40, y + 20);
      p->setBrush(QColor("purple"));
      p->setPoints(pa);
      p->setZ(0);
      p->show();

      if((text = new QCanvasText(boolean_symbols[i], canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      text->setColor(Qt::yellow);
      text->move(x + 3, y + 11);
      text->setZ(1);
      text->show();
      lines[0]->setPoints(x, y, x + 40, y + 20);
      lines[0]->show();
      lines[1]->setPoints(x + 40, y + 20, x, y + 40);
      lines[1]->show();
      lines[2]->setPoints(x, y, x, y + 40);
      lines[2]->show();
    }

  /*
  ** Draw a for-loop, a while-loop, a do-while-loop, and a repeat-loop.
  */

  for(i = 0, x = 5; i < 4; i++, x += square_s)
    if((rec = new QCanvasRectangle(x - 5 + 1, y + 44, square_s, square_s,
				   canvas)) != NULL)
      {
	rec->setPen(Qt::black);
	rec->setZ(0);
	rec->show();
      }
    else
      qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		       EXIT_FAILURE);

  for(i = 0, x = 5, y += 89; i < 4; i++, x += square_s)
    {
      if((p = new QCanvasPolygon(canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      fpa1[0] = QPoint(x, y);
      fpa1[1] = QPoint(x + 40, y);
      fpa1[2] = QPoint(x + 40, y - 40);
      fpa1[3] = QPoint(x, y - 40);
      p->setBrush(QColor("orange"));
      p->setPoints(fpa1);
      p->setZ(0);
      p->show();

      if(i == 0)
	{
	  if((text = new QCanvasText("N", canvas)) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  text->setColor(Qt::blue);
	  text->move(x + 25, y - 40);
	  text->setZ(1);
	  text->show();

	  if((text = new QCanvasText("i", canvas)) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  text->setColor(Qt::red);
	  text->move(x + 5, y - 20);
	  text->setZ(1);
	  text->show();
	}
      else if(i == 1)
	{
	  if((text = new QCanvasText("N", canvas)) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  text->setColor(Qt::blue);
	  text->move(x + 25, y - 40);
	  text->setZ(1);
	  text->show();

	  if((text = new QCanvasText("?", canvas)) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  text->setColor(Qt::red);
	  text->move(x + 5, y - 20);
	  text->setZ(1);
	  text->show();
	}
      else if(i == 2)
	{
	  if((text = new QCanvasText("T", canvas)) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  text->setColor(QColor("purple"));
	  text->move(x + 25, y - 40);
	  text->setZ(1);
	  text->show();

	  if((text = new QCanvasText("0", canvas)) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  text->setColor(Qt::red);
	  text->move(x + 5, y - 20);
	  text->setZ(1);
	  text->show();
	}
      else if(i == 3)
	{
	  if((text = new QCanvasText("T", canvas)) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  text->setColor(QColor("purple"));
	  text->move(x + 25, y - 40);
	  text->setZ(1);
	  text->show();

	  if((text = new QCanvasText("1", canvas)) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  text->setColor(Qt::red);
	  text->move(x + 5, y - 20);
	  text->setZ(1);
	  text->show();
	}

      for(j = 0; j < 4; j++)
	{
	  lines[j] = new QCanvasLine(canvas);

	  if(lines[j] == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  lines[j]->setPen(Qt::black);
	  lines[j]->setZ(1);
	}

      lines[0]->setPoints(x, y, x + 40, y);
      lines[0]->show();
      lines[1]->setPoints(x + 40, y, x + 40, y - 40);
      lines[1]->show();
      lines[2]->setPoints(x + 40, y - 40, x, y - 40);
      lines[2]->show();
      lines[3]->setPoints(x, y - 40, x, y);
      lines[3]->show();
    }

  /*
  ** Draw a subroutine, a break, a continue, and a return.
  */

  for(i = 0, x = 5; i < 4; i++, x += square_s)
    if((rec = new QCanvasRectangle(x - 5 + 1, y + 5, square_s, square_s,
				   canvas)) != NULL)
      {
	rec->setPen(Qt::black);
	rec->setZ(0);
	rec->show();
      }
    else
      qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		       EXIT_FAILURE);

  for(i = 0, x = 5, y += square_s; i < 4; i++, x += square_s)
    {
      if((p = new QCanvasPolygon(canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      fpa1[0] = QPoint(x, y);
      fpa1[1] = QPoint(x + 40, y);
      fpa1[2] = QPoint(x + 40, y - 40);
      fpa1[3] = QPoint(x, y - 40);

      if(strstr(other1_symbols[i], "GO") != NULL)
	p->setBrush(QColor("darkgreen"));
      else
	p->setBrush(QColor("tan"));

      p->setPoints(fpa1);
      p->setZ(0);
      p->show();

      if((text = new QCanvasText(other1_symbols[i], canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      if(i == 1)
	{
	  if((p = new QCanvasPolygon(canvas)) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  fpa2[0] = QPoint(x + 10, y - 5);
	  fpa2[1] = QPoint(x + 30, y - 5);
	  fpa2[2] = QPoint(x + 35, y - 10);
	  fpa2[3] = QPoint(x + 35, y - 30);
	  fpa2[4] = QPoint(x + 30, y - 35);
	  fpa2[5] = QPoint(x + 10, y - 35);
	  fpa2[6] = QPoint(x + 5, y - 30);
	  fpa2[7] = QPoint(x + 5, y - 10);
	  p->setBrush(Qt::red);
	  p->setPoints(fpa2);
	  p->setZ(5);
	  p->show();
	}

      if(strstr(other1_symbols[i], "GO") != NULL)
	text->setColor(QColor("yellow"));
      else
	text->setColor(QColor("darkBlue"));

      if(strstr(other1_symbols[i], "()") != NULL)
	text->move(x + 10, y - 30);
      else if(strstr(other1_symbols[i], "GO") != NULL)
	text->move(x + 5, y - 30);
      else
	text->move(x + 15, y - 30);

      text->setZ(10);
      text->show();

      for(j = 0; j < 4; j++)
	{
	  lines[j] = new QCanvasLine(canvas);

	  if(lines[j] == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  lines[j]->setPen(Qt::black);
	  lines[j]->setZ(1);
	}

      lines[0]->setPoints(x, y, x + 40, y);
      lines[0]->show();
      lines[1]->setPoints(x + 40, y, x + 40, y - 40);
      lines[1]->show();
      lines[2]->setPoints(x + 40, y - 40, x, y - 40);
      lines[2]->show();
      lines[3]->setPoints(x, y - 40, x, y);
      lines[3]->show();
    }

  /*
  ** Draw a constant, a variable, a task, and a function.
  */

  for(i = 0, x = 5; i < 4; i++, x += square_s)
    if((rec = new QCanvasRectangle(x - 5 + 1, y + 5, square_s, square_s,
				   canvas)) != NULL)
      {
	rec->setPen(Qt::black);
	rec->setZ(0);
	rec->show();
      }
    else
      qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		       EXIT_FAILURE);

  for(i = 0, x = 5, y += square_s; i < 4; i++, x += square_s)
    {
      if((p = new QCanvasPolygon(canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      fpa1[0] = QPoint(x, y);
      fpa1[1] = QPoint(x + 40, y);
      fpa1[2] = QPoint(x + 40, y - 40);
      fpa1[3] = QPoint(x, y - 40);
      p->setBrush(Qt::green);
      p->setPoints(fpa1);
      p->setZ(0);
      p->show();

      if((text = new QCanvasText(vars_symbols[i], canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      text->setColor(Qt::blue);

      if(strstr(vars_symbols[i], "()") != NULL)
	text->move(x + 10, y - 30);
      else
	text->move(x + 15, y - 30);

      text->setZ(1);
      text->show();

      for(j = 0; j < 4; j++)
	{
	  lines[j] = new QCanvasLine(canvas);

	  if(lines[j] == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  lines[j]->setPen(Qt::black);
	  lines[j]->setZ(1);
	}

      lines[0]->setPoints(x, y, x + 40, y);
      lines[0]->show();
      lines[1]->setPoints(x + 40, y, x + 40, y - 40);
      lines[1]->show();
      lines[2]->setPoints(x + 40, y - 40, x, y - 40);
      lines[2]->show();
      lines[3]->setPoints(x, y - 40, x, y);
      lines[3]->show();
    }

  /*
  ** Update the canvas.
  */

  canvas->update();
}

/*
** -- ~function_window() --
*/

function_window::~function_window()
{
  delete selection;
  selection = NULL;
  items.clear();
  descriptions.clear();
}

/*
** -- contentsMouseMoveEvent() --
*/

void function_window::contentsMouseMoveEvent(QMouseEvent *e)
{
  if(e == NULL)
    return;
}

/*
** -- contentsMousePressEvent() --
*/

void function_window::contentsMousePressEvent(QMouseEvent *e)
{
  int i = 0;
  int j = 0;
  int p = 0;

  if(e == NULL)
    return;
  else if(e->button() != Qt::LeftButton)
    return;

  type = -1;

  for(j = 0; j < (int) items.size() / ITEMS_COLS; j++)
    for(i = 0; i < ITEMS_COLS; i++)
      if(e->x() >= (square_s * i + 1) && e->x() <= (square_s * (i + 1) + 1))
	if(e->y() >= (square_s * j + 1) && e->y() <= (square_s * (j + 1) + 1))
	  {
	    p = i + ITEMS_COLS * j;
	    type = items[p];
	    selection->move(square_s * i, square_s * j);
	    goto done_label;
	  }

 done_label:

  if(type > -1)
    {
      ((QMainWindow *) parent)->statusBar()->message(descriptions[p]);
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

void function_window::displayDialog(void)
{
  parent->show();
}

/*
** -- hideDialog() --
*/

void function_window::hideDialog(void)
{
  parent->hide();
}

/*
** -- getType() --
*/

int function_window::getType(void)
{
  if(!selection->isVisible())
    return -1;
  else
    return type;
}

/*
** -- getParent() --
*/

QWidget *function_window::getParent(void)
{
  return parent;
}
