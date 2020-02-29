/*
** -- C Includes --
*/

/*
** -- C++ Includes --
*/

#include <math.h>
#include <iostream.h>

/*
** -- Qt Includes --
*/

#include <qstatusbar.h>
#include <qprogressdialog.h>

/*
** -- Local Includes --
*/

#include <gate.h>
#include <types.h>
#include <editor.h>
#include <qtnqc_main.h>
#include <rename_window.h>
#include <function_window.h>

extern qtnqc_main *qmain;
extern QApplication *qapp;
extern tools_window *tw;
extern function_window *fw;

template class QPtrList<gate>;
template class QPtrList<loop>;
template class QPtrList<task>;
template class QPtrList<constant>;
template class QPtrList<function>;
template class QPtrList<variable>;
template class QPtrList<subroutine>;
template class QValueListPrivate<QCanvasItem *>;

/*
** -- mousePressEvent() --
*/

void editor::mousePressEvent(QMouseEvent *e)
{  
  if(e == NULL)
    moving_start = QPoint();
  else if(e->button() == Qt::LeftButton)
    moving_start = e->pos();
  else
    moving_start = QPoint();
}

/*
** -- mouseMoveEvent() --
*/

void editor::mouseMoveEvent(QMouseEvent *e)
{
  if(tw->getType() == WIRE)
    /*
    ** Do not allow dragging if the user has selected the wire cursor.
    */

    return;

  if(e != NULL && (e->state() & Qt::LeftButton))
    if(QABS(e->pos().x() - moving_start.x()) >= 3 || QABS(e->pos().y() - moving_start.y()) >= 3)
      if(selected_item != NULL)
	if((drag = new dragobject(selected_item, (QWidget *) this, (const char *) NULL)) != NULL)
	  drag->dragMove();
	else
	  cerr << "Warning! Could not create a dragobject." << endl;
}

/*
** -- dropEvent() --
*/

void editor::dropEvent(QDropEvent *e)
{
  char buffer[256];
  gate *g = NULL;
  loop *l = NULL;
  task *t = NULL;
  QPoint pt;
  constant *c = NULL;
  function *f = NULL;
  variable *v = NULL;
  break_obj *b = NULL;
  QStatusBar *status_bar = ((QMainWindow *) dialog_parent)->statusBar();
  return_obj *r_o = NULL;
  subroutine *s = NULL;
  continue_obj *c_o = NULL;

  if(e == NULL)
    ;
  else if(dragobject::decode(e, selected_item_type))
    {
      pt = e->pos();

      /*
      ** Compensate for scrolling.
      */

      pt += QPoint(horizontalScrollBar()->value(),
		   verticalScrollBar()->value());

      /*
      ** Decline drops on existing objects.
      */

      if(collisionDetected(selected_item, pt))
	{
	  status_bar->message("Invalid drop location.", 3000);
	  goto return_label;
	}
      else
	status_bar->clear();

      if(selected_item_type == GATE)
	{
	  g = (gate *) selected_item;
	  g->move(pt);
	  selection->move(pt.x() - 5 - 15, pt.y() - 5 - 25);
	  selection->show();
	}
      else if(selected_item_type == LOOP)
	{
	  l = (loop *) selected_item;
	  l->move(pt);
	  selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
	  selection->show();
	}
      else if(selected_item_type == BREAK)
	{
	  b = (break_obj *) selected_item;
	  b->move(pt);
	  selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
	  selection->show();
	}
      else if(selected_item_type == CONTINUE)
	{
	  c_o = (continue_obj *) selected_item;
	  c_o->move(pt);
	  selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
	  selection->show();
	}
      else if(selected_item_type == RETURN)
	{
	  r_o = (return_obj *) selected_item;
	  r_o->move(pt);
	  selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
	  selection->show();
	}
      else if(selected_item_type == CONSTANT)
	{
	  c = (constant *) selected_item;
	  c->move(pt);
	  selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
	  selection->show();
	}
      else if(selected_item_type == VARIABLE)
	{
	  v = (variable *) selected_item;
	  v->move(pt);
	  selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
	  selection->show();
	}
      else if(selected_item_type == TASK)
	{
	  t = (task *) selected_item;
	  t->move(pt);
	  selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
	  selection->show();
	}
      else if(selected_item_type == SUBROUTINE)
	{
	  s = (subroutine *) selected_item;
	  s->move(pt);
	  selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
	  selection->show();
	}
      else if(selected_item_type == FUNCTION)
	{
	  f = (function *) selected_item;
	  f->move(pt);
	  selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
	  selection->show();
	}

      canvas()->update();

      if(selection->isVisible())
	{
	  (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
			  selected_item->getX(),
			  selected_item->getY());
	  status_bar->message(buffer);
	  updateLister();
	}

      qmain->setSaved(false);
    }

 return_label:

  delete drag;
  drag = NULL;
}

/*
** -- dragEnterEvent() --
*/

void editor::dragEnterEvent(QDragEnterEvent *e)
{
  if(e == NULL)
    ;
  else if(e->source() != (QWidget *) this)
    e->ignore();
  else if(dragobject::canDecode(e))
    e->accept();
  else
    e->ignore();
}

/*
** -- contentsMousePressEvent() --
*/

void editor::contentsMousePressEvent(QMouseEvent *e)
{
  int i = 0;
  int function_type = 0;
  bool rightmouse = false;
  bool object_was_deleted = false;
  char buffer[256];
  QKeyEvent my_e(QEvent::KeyPress, 0, 127, 0); /*
					       ** The second argument (0) is used for
					       ** deciding whether or not to call
					       ** askAboutDelete() in keyPressEvent().
					       */
  QStatusBar *status_bar = ((QMainWindow *) dialog_parent)->statusBar();
  QCanvasItemList hits;

  /*
  ** Method functionality:
  ** 1. This method is to be used for adding new objects to the canvas or
  **    selecting existing objects.
  ** 2. Display a submenu when the user right-clicks on the canvas.
  */

  if(e == NULL)
    return;
  else if(e->button() == Qt::RightButton)
    rightmouse = true;
  else if(e->button() != Qt::LeftButton)
    return;
  else if(e->button() == Qt::LeftButton && tw->getType() == -1)
    return;

  setFocus();
  last_pos = e->pos();
  updateFuncSubmenu();
  updateTaskSubmenu();
  updateSubroutineSubmenu();
  editmenu->setItemEnabled(editmenu->idAt(0), false);
  editmenu->setItemEnabled(editmenu->idAt(1), true);
  editmenu->setItemEnabled(editmenu->idAt(2), true);
  editmenu->setItemEnabled(editmenu->idAt(4), true);
  editmenu->setItemEnabled(editmenu->idAt(6), true);
  editmenu->setItemEnabled(editmenu->idAt(7), true);
  editmenu->setItemEnabled(editmenu->idAt(8), true);
  editmenu->setItemEnabled(editmenu->idAt(10), true);
  editmenu->setItemEnabled(editmenu->idAt(16), false);
  editmenu->setItemEnabled(editmenu->idAt(18), false);
  editmenu->setItemEnabled(editmenu->idAt(19), false);

  if(canvas()->allItems().size() == 1)
    editmenu->setItemEnabled(editmenu->idAt(4), false);

  if(qmain->getUsedFunctionNames().isEmpty())
    editmenu->setItemEnabled(editmenu->idAt(6), false);

  if(qmain->getUsedSubroutineNames().isEmpty())
    editmenu->setItemEnabled(editmenu->idAt(7), false);

  if(qmain->getUsedTaskNames().isEmpty())
    editmenu->setItemEnabled(editmenu->idAt(8), false);

  selected_item = NULL;
  selected_item_type = -1;
  function_type = fw->getType();
  hits = canvas()->collisions(e->pos());

  if(hits.size() > 0)
    {
      /*
      ** Item selected.
      */

      editmenu->setItemEnabled(editmenu->idAt(0), true);
      editmenu->setItemEnabled(editmenu->idAt(6), false);
      editmenu->setItemEnabled(editmenu->idAt(7), false);
      editmenu->setItemEnabled(editmenu->idAt(8), false);

      for(i = 0; i < (int) gates->count(); i++)
	if(hits.contains(gates->at(i)->getPolygon()))
	  {
	    selected_item_type = GATE;
	    selected_item = (object *) gates->at(i);
	    selection->move(gates->at(i)->getX() - 5,
			    gates->at(i)->getY() - 5);
	    selection->setSize(60, 60);
	    selection->show();

	    if(tw->getType() != DELETE)
	      {
		(void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
				selected_item->getX(),
				selected_item->getY());
		status_bar->message(buffer);
	      }

	    if(tw->getType() == WIRE)
	      {
		if(sel_timer != NULL)
		  sel_timer->start(500);
	      }
	    else if(tw->getType() == DELETE)
	      {
		if((object_was_deleted = askAboutDelete()))
		  keyPressEvent(&my_e);

		goto return_label;
	      }
	    else
	      {
		if(sel_timer != NULL)
		  sel_timer->stop();
	      }

	    canvas()->update();
	    break;
	  }

      if(selected_item_type == GATE && rightmouse)
	{
	  editmenu->setItemEnabled(editmenu->idAt(1), false);
	  editmenu->setItemEnabled(editmenu->idAt(2), false);
	  editmenu->setItemEnabled(editmenu->idAt(18), true);
	  editmenu->exec(QCursor::pos());
	  goto return_label;
	}

      if(selected_item_type == -1)
	{
	  for(i = 0; i < (int) loops->count(); i++)
	    if(hits.contains(loops->at(i)->getPolygon()))
	      {
		selected_item_type = LOOP;
		selected_item = (object *) loops->at(i);
		selection->move(loops->at(i)->getX() - 5,
				loops->at(i)->getY() - 5);
		selection->setSize(60, 60);
		selection->show();

		if(tw->getType() != DELETE)
		  {
		    (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
				    selected_item->getX(),
				    selected_item->getY());
		    status_bar->message(buffer);
		  }

		if(tw->getType() == WIRE)
		  {
		    if(sel_timer != NULL)
		      sel_timer->start(500);
		  }
		else if(tw->getType() == DELETE)
		  {
		    if((object_was_deleted = askAboutDelete()))
		      keyPressEvent(&my_e);

		    goto return_label;
		  }
		else
		  {
		    if(sel_timer != NULL)
		      sel_timer->stop();
		  }

		canvas()->update();
		break;
	      }

	  /*
	  ** Display an edit menu if the user selected a
	  ** loop structure.
	  */

	  if(selected_item_type == LOOP && rightmouse)
	    {
	      editmenu->setItemEnabled(editmenu->idAt(2), false);
	      editmenu->setItemEnabled(editmenu->idAt(18), true);
	      editmenu->exec(QCursor::pos());
	      goto return_label;
	    }
	}

      if(selected_item_type == -1)
	{
	  for(i = 0; i < (int) constants->count(); i++)
	    if(hits.contains(constants->at(i)->getPolygon()))
	      {
		selected_item_type = CONSTANT;
		selected_item = (object *) constants->at(i);
		selection->move(constants->at(i)->getX() - 5,
				constants->at(i)->getY() - 5);
		selection->setSize(60, 60);
		selection->show();

		if(tw->getType() != DELETE)
		  {
		    (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
				    selected_item->getX(),
				    selected_item->getY());
		    status_bar->message(buffer);
		  }

		if(tw->getType() == WIRE)
		  {
		    if(sel_timer != NULL)
		      sel_timer->start(500);
		  }
		else if(tw->getType() == DELETE)
		  {
		    if((object_was_deleted = askAboutDelete()))
		      keyPressEvent(&my_e);

		    goto return_label;
		  }
		else
		  {
		    if(sel_timer != NULL)
		      sel_timer->stop();
		  }

		canvas()->update();
		break;
	      }

	  /*
	  ** Display an edit menu if the user selected a
	  ** constant structure.
	  */

	  if(selected_item_type == CONSTANT && rightmouse)
	    {
	      editmenu->setItemEnabled(editmenu->idAt(2), false);
	      editmenu->setItemEnabled(editmenu->idAt(18), true);
	      editmenu->exec(QCursor::pos());
	      goto return_label;
	    }
	}

      /*
      ** Variables can only be 16-bit integers.
      ** So why allow the user to modify them?
      */

      if(selected_item_type == -1)
	{
	  for(i = 0; i < (int) variables->count(); i++)
	    if(hits.contains(variables->at(i)->getPolygon()))
	      {
		selected_item_type = VARIABLE;
		selected_item = (object *) variables->at(i);
		selection->move(variables->at(i)->getX() - 5,
				variables->at(i)->getY() - 5);
		selection->setSize(60, 60);
		selection->show();

		if(tw->getType() != DELETE)
		  {
		    (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
				    selected_item->getX(),
				    selected_item->getY());
		    status_bar->message(buffer);
		  }

		if(tw->getType() == WIRE)
		  {
		    if(sel_timer != NULL)
		      sel_timer->start(500);
		  }
		else if(tw->getType() == DELETE)
		  {
		    if((object_was_deleted = askAboutDelete()))
		      keyPressEvent(&my_e);

		    goto return_label;
		  }
		else
		  {
		    if(sel_timer != NULL)
		      sel_timer->stop();
		  }

		canvas()->update();
		break;
	      }

	  if(selected_item_type == VARIABLE && rightmouse)
	    {
	      editmenu->setItemEnabled(editmenu->idAt(1), false);
	      editmenu->setItemEnabled(editmenu->idAt(2), true);

	      if(!isMainEditor())
		{
		  editmenu->setItemEnabled(editmenu->idAt(16), true);

		  if(((variable *) selected_item)->isGlobal())
		    editmenu->changeItem(editmenu->idAt(16), "Set As Local");
		  else
		    editmenu->changeItem(editmenu->idAt(16), "Set As Global");
		}

	      editmenu->setItemEnabled(editmenu->idAt(18), true);
	      editmenu->exec(QCursor::pos());
	      goto return_label;
	    }
	}

      if(selected_item_type == -1)
	{
	  for(i = 0; i < (int) tasks->count(); i++)
	    if(hits.contains(tasks->at(i)->getPolygon()))
	      {
		selected_item_type = TASK;
		selected_item = (object *) tasks->at(i);
		selection->move(tasks->at(i)->getX() - 5,
				tasks->at(i)->getY() - 5);
		selection->setSize(60, 60);
		selection->show();

		if(tw->getType() != DELETE)
		  {
		    (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
				    selected_item->getX(),
				    selected_item->getY());
		    status_bar->message(buffer);
		  }

		if(tw->getType() == WIRE)
		  {
		    if(sel_timer != NULL)
		      sel_timer->start(500);
		  }
		else if(tw->getType() == DELETE)
		  {
		    if((object_was_deleted = askAboutDelete()))
		      keyPressEvent(&my_e);

		    goto return_label;
		  }
		else
		  {
		    if(sel_timer != NULL)
		      sel_timer->stop();
		  }

		canvas()->update();
		break;
	      }

	  /*
	  ** Display an edit menu if the user selected a
	  ** task structure.
	  */

	  if(selected_item_type == TASK && rightmouse)
	    {
	      if(!isMainEditor())
		editmenu->setItemEnabled(editmenu->idAt(2), false);

	      editmenu->exec(QCursor::pos());
	      goto return_label;
	    }
	}

      if(selected_item_type == -1)
	{
	  for(i = 0; i < (int) subroutines->count(); i++)
	    if(hits.contains(subroutines->at(i)->getPolygon()))
	      {
		selected_item_type = SUBROUTINE;
		selected_item = (object *) subroutines->at(i);
		selection->move(subroutines->at(i)->getX() - 5,
				subroutines->at(i)->getY() - 5);
		selection->setSize(60, 60);
		selection->show();

		if(tw->getType() != DELETE)
		  {
		    (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
				    selected_item->getX(),
				    selected_item->getY());
		    status_bar->message(buffer);
		  }

		if(tw->getType() == WIRE)
		  {
		    if(sel_timer != NULL)
		      sel_timer->start(500);
		  }
		else if(tw->getType() == DELETE)
		  {
		    if((object_was_deleted = askAboutDelete()))
		      keyPressEvent(&my_e);

		    goto return_label;
		  }
		else
		  {
		    if(sel_timer != NULL)
		      sel_timer->stop();
		  }

		canvas()->update();
		break;
	      }

	  /*
	  ** Display an edit menu if the user selected a
	  ** task structure.
	  */

	  if(selected_item_type == SUBROUTINE && rightmouse)
	    {
	      if(!isMainEditor())
		editmenu->setItemEnabled(editmenu->idAt(2), false);

	      editmenu->exec(QCursor::pos());
	      goto return_label;
	    }
	}

      if(selected_item_type == -1)
	{
	  for(i = 0; i < (int) functions->count(); i++)
	    if(hits.contains(functions->at(i)->getPolygon()))
	      {
		selected_item_type = FUNCTION;
		selected_item = (object *) functions->at(i);
		selection->move(functions->at(i)->getX() - 5,
				functions->at(i)->getY() - 5);
		selection->setSize(60, 60);
		selection->show();

		if(tw->getType() != DELETE)
		  {
		    (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
				    selected_item->getX(),
				    selected_item->getY());
		    status_bar->message(buffer);
		  }

		if(tw->getType() == WIRE)
		  {
		    if(sel_timer != NULL)
		      sel_timer->start(500);
		  }
		else if(tw->getType() == DELETE)
		  {
		    if((object_was_deleted = askAboutDelete()))
		      keyPressEvent(&my_e);

		    goto return_label;
		  }
		else
		  {
		    if(sel_timer != NULL)
		      sel_timer->stop();
		  }

		canvas()->update();
		break;
	      }

	  /*
	  ** Display an edit menu if the user selected a
	  ** function structure.
	  */

	  if(selected_item_type == FUNCTION && rightmouse)
	    {
	      if(!isMainEditor())
		editmenu->setItemEnabled(editmenu->idAt(2), false);

	      editmenu->exec(QCursor::pos());
	      goto return_label;
	    }
	}

      /*
      ** Breaks.
      */

      if(selected_item_type == -1)
	{
	  for(i = 0; i < (int) breaks->count(); i++)
	    if(hits.contains(breaks->at(i)->getPolygon()))
	      {
		selected_item_type = BREAK;
		selected_item = (object *) breaks->at(i);
		selection->move(breaks->at(i)->getX() - 5,
				breaks->at(i)->getY() - 5);
		selection->setSize(60, 60);
		selection->show();

		if(tw->getType() != DELETE)
		  {
		    (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
				    selected_item->getX(),
				    selected_item->getY());
		    status_bar->message(buffer);
		  }

		if(tw->getType() == WIRE)
		  {
		    if(sel_timer != NULL)
		      sel_timer->start(500);
		  }
		else if(tw->getType() == DELETE)
		  {
		    if((object_was_deleted = askAboutDelete()))
		      keyPressEvent(&my_e);

		    goto return_label;
		  }
		else
		  {
		    if(sel_timer != NULL)
		      sel_timer->stop();
		  }

		canvas()->update();
		break;
	      }

	  /*
	  ** Display an edit menu if the user selected a
	  ** break structure.
	  */

	  if(selected_item_type == BREAK && rightmouse)
	    {
	      editmenu->setItemEnabled(editmenu->idAt(1), false);
	      editmenu->setItemEnabled(editmenu->idAt(2), false);
	      editmenu->setItemEnabled(editmenu->idAt(18), true);
	      editmenu->exec(QCursor::pos());
	      goto return_label;
	    }
	}

      /*
      ** Continues.
      */

      if(selected_item_type == -1)
	{
	  for(i = 0; i < (int) continues->count(); i++)
	    if(hits.contains(continues->at(i)->getPolygon()))
	      {
		selected_item_type = CONTINUE;
		selected_item = (object *) continues->at(i);
		selection->move(continues->at(i)->getX() - 5,
				continues->at(i)->getY() - 5);
		selection->setSize(60, 60);
		selection->show();

		if(tw->getType() != DELETE)
		  {
		    (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
				    selected_item->getX(),
				    selected_item->getY());
		    status_bar->message(buffer);
		  }

		if(tw->getType() == WIRE)
		  {
		    if(sel_timer != NULL)
		      sel_timer->start(500);
		  }
		else if(tw->getType() == DELETE)
		  {
		    if((object_was_deleted = askAboutDelete()))
		      keyPressEvent(&my_e);

		    goto return_label;
		  }
		else
		  {
		    if(sel_timer != NULL)
		      sel_timer->stop();
		  }

		canvas()->update();
		break;
	      }

	  /*
	  ** Display an edit menu if the user selected a
	  ** continue structure.
	  */

	  if(selected_item_type == CONTINUE && rightmouse)
	    {
	      editmenu->setItemEnabled(editmenu->idAt(1), false);
	      editmenu->setItemEnabled(editmenu->idAt(2), false);
	      editmenu->setItemEnabled(editmenu->idAt(18), true);
	      editmenu->exec(QCursor::pos());
	      goto return_label;
	    }
	}

      /*
      ** Returns.
      */

      if(selected_item_type == -1)
	{
	  for(i = 0; i < (int) returns->count(); i++)
	    if(hits.contains(returns->at(i)->getPolygon()))
	      {
		selected_item_type = RETURN;
		selected_item = (object *) returns->at(i);
		selection->move(returns->at(i)->getX() - 5,
				returns->at(i)->getY() - 5);
		selection->setSize(60, 60);
		selection->show();

		if(tw->getType() != DELETE)
		  {
		    (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
				    selected_item->getX(),
				    selected_item->getY());
		    status_bar->message(buffer);
		  }

		if(tw->getType() == WIRE)
		  {
		    if(sel_timer != NULL)
		      sel_timer->start(500);
		  }
		else if(tw->getType() == DELETE)
		  {
		    if((object_was_deleted = askAboutDelete()))
		      keyPressEvent(&my_e);

		    goto return_label;
		  }
		else
		  {
		    if(sel_timer != NULL)
		      sel_timer->stop();
		  }

		canvas()->update();
		break;
	      }

	  /*
	  ** Display an edit menu if the user selected a
	  ** return structure.
	  */

	  if(selected_item_type == RETURN && rightmouse)
	    {
	      editmenu->setItemEnabled(editmenu->idAt(1), false);
	      editmenu->setItemEnabled(editmenu->idAt(2), false);
	      editmenu->setItemEnabled(editmenu->idAt(18), true);
	      editmenu->exec(QCursor::pos());
	      goto return_label;
	    }
	}
    }
  else if(function_type > -1 && !rightmouse)
    {
      if(tw->getType() == DELETE)
	goto return_label;

      if(collisionDetected(selected_item, e->pos()))
	{
	  status_bar->message("Invalid insert location.", 3000);
	  goto return_label;
	}
      else
	status_bar->clear();

      switch(function_type)
	{
	case ADD_GATE:
	case DIVIDE_GATE:
	case MULTIPLY_GATE:
	case SUBTRACT_GATE:
	case NOT_GATE:
	case AND_GATE:
	case OR_GATE:
	case EQUALS_GATE:
	case GREATER_THAN_GATE:
	case LESS_THAN_GATE:
	case GREATER_THAN_OR_EQUAL_TO_GATE:
	case LESS_THAN_OR_EQUAL_TO_GATE:
	case NOT_EQUAL_TO_GATE:
	case BITWISE_AND_GATE:
	case BITWISE_XOR_GATE:
	case BITWISE_OR_GATE:
	case UNARY_MINUS_GATE:
	case BITWISE_NEGATION_GATE:
	case MODULO_GATE:
	case INCREMENT_GATE:
	case DECREMENT_GATE:
	case LEFT_SHIFT_GATE:
	case RIGHT_SHIFT_GATE:
	case ABSOLUTE_GATE:
	case SIGN_OF_GATE:
	case LESS_THAN_ZERO_GATE:
	case GREATER_THAN_ZERO_GATE:
	case GREATER_THAN_ONE_GATE:
	  {
	    drawGate(e->x(), e->y(), function_type);
	    break;
	  }
	case FOR_LOOP:
	case WHILE_LOOP:
	case DO_WHILE_LOOP:
	case REPEAT_LOOP:
	  {
	    drawLoop(e->x(), e->y(), function_type, NULL, -1, "",
		     editor::MAX_WIDTH, editor::MAX_HEIGHT);
	    break;
	  }
	case CONSTANT:
	  {
	    drawConstant(e->x(), e->y(), function_type);
	    break;
	  }
	case VARIABLE:
	  {
	    drawVariable(e->x(), e->y(), function_type, false, -1, "");
	    break;
	  }
	case BREAK:
	  {
	    drawBreak(e->x(), e->y(), -1);
	    break;
	  }
	case CONTINUE:
	  {
	    drawContinue(e->x(), e->y(), -1);
	    break;
	  }
	case RETURN:
	  {
	    drawReturn(e->x(), e->y(), -1);
	    break;
	  }
	case SUBROUTINE:
	  {
	    if(isMainEditor())
	      {
		if(subroutines->count() < MAX_SUBROUTINES)
		  drawSubroutine(e->x(), e->y(), function_type, NULL);
		else
		  {
		    (void) snprintf(buffer, sizeof(buffer),
				    "Maximum (%d) number of defined "
				    "subroutines has been "
				    "reached.", MAX_SUBROUTINES);
		    status_bar->message(buffer, 3000);
		  }
	      }
	    else if(status_bar != NULL)
	      status_bar->message("New subroutines may only be added to "
				  "the main canvas.", 3000);

	    break;
	  }
	case TASK:
	  {
	    if(isMainEditor())
	      {
		if(tasks->count() < MAX_TASKS)
		  drawTask(e->x(), e->y(), function_type, NULL);
		else
		  {
		    (void) snprintf(buffer, sizeof(buffer),
				    "Maximum (%d) number of defined tasks "
				    "has been reached.", MAX_TASKS);
		    status_bar->message(buffer, 3000);
		  }
	      }
	    else if(status_bar != NULL)
	      status_bar->message("New tasks may only be added to "
				  "the main canvas.", 3000);

	    break;
	  }
	case FUNCTION:
	  {
	    if(isMainEditor())
	      {
		if(functions->count() < MAX_FUNCTIONS)
		  drawFunction(e->x(), e->y(), function_type,
			       editor::MAX_WIDTH, editor::MAX_HEIGHT);
		else
		  {
		    (void) snprintf(buffer, sizeof(buffer),
				    "Maximum (%d) number of defined "
				    "has been functions reached.",
				    MAX_FUNCTIONS);
		    status_bar->message(buffer, 3000);
		  }
	      }
	    else if(status_bar != NULL)
	      status_bar->message("New functions may only added to "
				  "the main canvas.", 3000);

	    break;
	  }
	default:
	  break;
	}

      if(tw->getType() == WIRE && sel_timer != NULL)
	sel_timer->stop();

      selection->hide();
      canvas()->update();
    }
  else if(!rightmouse)
    {
      if(sel_timer != NULL)
	sel_timer->stop();

      selection->hide();
      status_bar->clear();
      canvas()->update();
    }
  else if(rightmouse)
    {
      /*
      ** Display a popup menu.
      */

      editmenu->setItemEnabled(editmenu->idAt(1), false);
      editmenu->setItemEnabled(editmenu->idAt(2), false);

      if(copied_item_type > -1 && copied_item != NULL)
	editmenu->setItemEnabled(editmenu->idAt(19), true);

      editmenu->exec(QCursor::pos());
    }

 return_label:

  if(tw->getType() == DELETE && object_was_deleted)
    status_bar->clear();
  else if(tw->getType() == CURSOR)
    {
      /*
      ** Seems to cause problems.
      */

#if 0
      if(e->type() == QEvent::MouseButtonDblClick)
	slotEdit();
#endif
    }

  hits.clear();
}

/*
** -- editor() --
*/

editor::editor(const bool ismain, QCanvas *canvas, const int parent_id_arg,
	       QWidget *parent,
	       QWidget *dialog_parent_arg, const char *name, WFlags f):
  QCanvasView(canvas, parent, name, f)
{
  dialog_parent = dialog_parent_arg;
  canvas->setBackgroundColor(QColor("lavender"));
  is_main_editor = ismain;
  canvas->setDoubleBuffering(true);
  setCanvas(canvas);
  setFocusPolicy(QWidget::StrongFocus);
  selected_item_type = -1;
  gates = NULL;
  loops = NULL;
  tasks = NULL;
  subroutines = NULL;
  wires = NULL;
  constants = NULL;
  functions = NULL;
  variables = NULL;
  breaks = NULL;
  editmenu = NULL;
  func_submenu = NULL;
  task_submenu = NULL;
  subroutine_submenu = NULL;
  drag = NULL;
  selection = NULL;
  sel_timer = NULL;
  parent_id = parent_id_arg;
  copied_item = NULL;
  copied_item_type = -1;

  if((selection = new QCanvasRectangle(0, 0, 55, 55, canvas)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  selection->setZ(1);
  selection->setPen(QPen(Qt::red, 0, QPen::DashLine));

  if((sel_timer = new QTimer(this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  connect(sel_timer, SIGNAL(timeout(void)), this,
	  SLOT(slotSelTimerTimeout(void)));

  if((editmenu = new QPopupMenu()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((func_submenu = new QPopupMenu()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((task_submenu = new QPopupMenu()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((subroutine_submenu = new QPopupMenu()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  editmenu->insertItem("&Delete Object", this, SLOT(slotDelete(void)));
  editmenu->insertItem("&Edit Object", this, SLOT(slotEdit(void)));
  editmenu->insertItem("&Rename Object", this, SLOT(slotRename(void)));
  editmenu->insertSeparator();
  editmenu->insertItem("&List Canvas Items", this,
		       SLOT(slotListAll(void)));
  editmenu->insertSeparator();
  editmenu->insertItem("Add Existing &Function ", func_submenu);
  editmenu->insertItem("Add Existing &Subroutine ", subroutine_submenu);
  editmenu->insertItem("Add Existing &Task ", task_submenu);
  editmenu->insertSeparator();
  editmenu->insertItem("&Help", this, SLOT(slotShowHelp(void)));
  editmenu->insertSeparator();
  editmenu->insertItem("&Raise Main Window", this, SLOT(slotShowMain(void)));
  editmenu->insertItem("&Show Functions Palette", this,
		       SLOT(slotShowFuncs(void)));
  editmenu->insertItem("Show &Tools Palette", this,
		       SLOT(slotShowTools(void)));
  editmenu->insertSeparator();
  editmenu->insertItem("&Set As Global", this, SLOT(setGlobal(void)));
  editmenu->insertSeparator();
  editmenu->insertItem("&Copy Object", this,
		       SLOT(slotCopyObject(void)));
  editmenu->insertItem("&Paste Object", this,
		       SLOT(slotPasteObject(void)));

  if((gates = new QPtrList<gate>()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((breaks = new QPtrList<break_obj>()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((continues = new QPtrList<continue_obj>()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((returns = new QPtrList<return_obj>()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((loops = new QPtrList<loop>()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((tasks = new QPtrList<task>()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((subroutines = new QPtrList<subroutine>()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((constants = new QPtrList<constant>()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((functions = new QPtrList<function>()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((variables = new QPtrList<variable>()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((wires = new QPtrList<wire>()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((lister = new list_objects(parent, this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((rw = new rename_window(parent)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  /*
  ** Set AutoDelete to true so that an entry's destructor
  ** is called upon the entry's removal from the list.
  */

  gates->setAutoDelete(true);
  loops->setAutoDelete(true);
  tasks->setAutoDelete(true);
  subroutines->setAutoDelete(true);
  breaks->setAutoDelete(true);
  continues->setAutoDelete(true);
  returns->setAutoDelete(true);
  functions->setAutoDelete(true);
  constants->setAutoDelete(true);
  wires->setAutoDelete(true);
  variables->setAutoDelete(true);

  /*
  ** Accept drops.
  */

  setAcceptDrops(true);
  setDragAutoScroll(true);
}

/*
** -- cleanup() --
*/

void editor::cleanup(void)
{
  QStatusBar *status_bar = ((QMainWindow *) dialog_parent)->statusBar();

  /*
  ** Use this method to remove all objects from a canvas.
  */

  if(gates != NULL)
    gates->clear();

  if(breaks != NULL)
    breaks->clear();

  if(continues != NULL)
    continues->clear();

  if(returns != NULL)
    returns->clear();

  if(loops != NULL)
    loops->clear();

  if(tasks != NULL)
    tasks->clear();

  if(subroutines != NULL)
    subroutines->clear();

  if(functions != NULL)
    functions->clear();

  if(constants != NULL)
    constants->clear();

  if(wires != NULL)
    wires->clear();

  if(variables != NULL)
    variables->clear();

  if(sel_timer != NULL)
    sel_timer->stop();

  if(status_bar != NULL)
    status_bar->clear();

  if(selection != NULL)
    selection->hide();

  if(lister != NULL)
    {
      lister->hide();
      lister->clear();
    }

  if(copied_item != NULL)
    {
      delete copied_item;
      copied_item = NULL;
    }

  selected_item = NULL;
  copied_item_type = selected_item_type = -1;
  used_variable_names.clear();

  /*
  ** Scroll to (0,0).
  */

  setContentsPos(0, 0);

  if(canvas() != NULL)
    {
      canvas()->resize(editor::MAX_WIDTH, editor::MAX_HEIGHT);
      canvas()->update();
    }

  qapp->processEvents();
}

/*
** -- ~editor() --
*/

editor::~editor()
{
  cleanup();
  delete gates;
  delete breaks;
  delete continues;
  delete returns;
  delete loops;
  delete tasks;
  delete subroutines;
  delete functions;
  delete constants;
  delete wires;
  delete variables;
  delete editmenu;
  delete func_submenu;
  delete task_submenu;
  delete subroutine_submenu;
  delete selection;
  delete lister;
  delete rw;
  delete copied_item;
  rw = NULL;
  lister = NULL;
  gates = NULL;
  breaks = NULL;
  continues = NULL;
  returns = NULL;
  loops = NULL;
  tasks = NULL;
  subroutines = NULL;
  functions = NULL;
  constants = NULL;
  wires = NULL;
  variables = NULL;
  editmenu = NULL;
  func_submenu = NULL;
  task_submenu = NULL;
  subroutine_submenu = NULL;
  selection = NULL;
  copied_item = NULL;
  copied_item_type = selected_item_type = -1;
}

/*
** -- drawTask() --
*/

void editor::drawTask(const int x0, const int y0, const int type,
		      QPtrList<file_class> *list, const int id_arg,
		      const QString &name_arg, const int cwidth,
		      const int cheight)
{
  int x = 0;
  int y = 0;
  task *t = NULL;

  x = x0 - 25;
  y = y0 - 25;

  if(type == TASK)
    t = new task(x, y, canvas(), type, list, id_arg, name_arg,
		 cwidth, cheight);

  if(t != NULL)
    {
      tasks->append(t);
      qmain->setSaved(false);
    }
  else
    cerr << "Warning! Could not insert object into task list. "
	 << "File " << __FILE__ << ", "
	 << "line " << __LINE__ << "." << endl;

  updateLister();
}

/*
** -- drawSubroutine() --
*/

void editor::drawSubroutine(const int x0, const int y0, const int type,
			    QPtrList<file_class> *list, const int id_arg,
			    const QString &name_arg, const int cwidth,
			    const int cheight)
{
  int x = 0;
  int y = 0;
  subroutine *s = NULL;

  x = x0 - 25;
  y = y0 - 25;

  if(type == SUBROUTINE)
    s = new subroutine(x, y, canvas(), type, list, id_arg, name_arg,
		       cwidth, cheight);

  if(s != NULL)
    {
      subroutines->append(s);
      qmain->setSaved(false);
    }
  else
    cerr << "Warning! Could not insert object into subroutine list. "
	 << "File " << __FILE__ << ", "
	 << "line " << __LINE__ << "." << endl;

  updateLister();
}

/*
** -- drawGate() --
*/

void editor::drawGate(const int x0, const int y0, const int type,
		      const int id_arg)
{
  int x = 0;
  int y = 0;
  gate *g = NULL;

  x = x0 - 15;
  y = y0 - 25;

  if(type == ADD_GATE)
    g = new gate(x, y, "+", canvas(), type, "Add Gate", id_arg);
  else if(type == SUBTRACT_GATE)
    g = new gate(x, y, "-", canvas(), type, "Subtract Gate", id_arg);
  else if(type == MULTIPLY_GATE)
    g = new gate(x, y, "*", canvas(), type, "Multiply Gate", id_arg);
  else if(type == DIVIDE_GATE)
    g = new gate(x, y, "/", canvas(), type, "Divide Gate", id_arg);
  else if(type == NOT_GATE)
    g = new gate(x, y, "!", canvas(), type, "Not Gate", id_arg);
  else if(type == AND_GATE)
    g = new gate(x, y, "&&", canvas(), type, "And Gate", id_arg);
  else if(type == OR_GATE)
    g = new gate(x, y, "||", canvas(), type, "Or Gate", id_arg);
  else if(type == EQUALS_GATE)
    g = new gate(x, y, "==", canvas(), type, "Equals Gate", id_arg);
  else if(type == GREATER_THAN_GATE)
    g = new gate(x, y, ">", canvas(), type, "Greater Than Gate", id_arg);
  else if(type == LESS_THAN_GATE)
    g = new gate(x, y, "<", canvas(), type, "Less Than Gate", id_arg);
  else if(type == GREATER_THAN_OR_EQUAL_TO_GATE)
    g = new gate(x, y, ">=", canvas(), type,
		 "Greater Than Or Equal To Gate", id_arg);
  else if(type == LESS_THAN_OR_EQUAL_TO_GATE)
    g = new gate(x, y, "<=", canvas(), type, "Less Than Or Equal To Gate",
		 id_arg);
  else if(type == NOT_EQUAL_TO_GATE)
    g = new gate(x, y, "!=", canvas(), type, "Not Equal To Gate", id_arg);
  else if(type == BITWISE_AND_GATE)
    g = new gate(x, y, "&", canvas(), type, "Bitwise And Gate", id_arg);
  else if(type == BITWISE_XOR_GATE)
    g = new gate(x, y, "^", canvas(), type, "Bitwise Xor Gate", id_arg);
  else if(type == BITWISE_OR_GATE)
    g = new gate(x, y, "|", canvas(), type, "Bitwise Or Gate", id_arg);
  else if(type == UNARY_MINUS_GATE)
    g = new gate(x, y, "-x", canvas(), type, "Unary Minus Gate", id_arg);
  else if(type == BITWISE_NEGATION_GATE)
    g = new gate(x, y, "~", canvas(), type, "Bitwise Negation Gate", id_arg);
  else if(type == INCREMENT_GATE)
    g = new gate(x, y, "++", canvas(), type, "Increment Gate", id_arg);
  else if(type == DECREMENT_GATE)
    g = new gate(x, y, "--", canvas(), type, "Decrement Gate", id_arg);
  else if(type == MODULO_GATE)
    g = new gate(x, y, "%", canvas(), type, "Modulo Gate", id_arg);
  else if(type == LEFT_SHIFT_GATE)
    g = new gate(x, y, "<<", canvas(), type, "Left Shift Gate", id_arg);
  else if(type == RIGHT_SHIFT_GATE)
    g = new gate(x, y, ">>", canvas(), type, "Right Shift Gate", id_arg);
  else if(type == ABSOLUTE_GATE)
    g = new gate(x, y, "[]", canvas(), type, "Absolute Gate", id_arg);
  else if(type == SIGN_OF_GATE)
    g = new gate(x, y, "-?", canvas(), type, "Sign Of Gate", id_arg);
  else if(type == LESS_THAN_ZERO_GATE)
    g = new gate(x, y, "<0", canvas(), type, "Less Than Zero Gate", id_arg);
  else if(type == GREATER_THAN_ZERO_GATE)
    g = new gate(x, y, ">0", canvas(), type, "Greater Than Zero Gate", id_arg);
  else if(type == GREATER_THAN_ONE_GATE)
    g = new gate(x, y, ">1", canvas(), type, "Greater Than One Gate", id_arg);

  if(gates != NULL)
    {
      gates->append(g);
      qmain->setSaved(false);
    }
  else
    cerr << "Warning! Could not insert object into gate list. "
	 << "File " << __FILE__ << ", "
	 << "line " << __LINE__ << "." << endl;

  updateLister();
}

/*
** -- drawBreak() --
*/

void editor::drawBreak(const int x0, const int y0, const int id_arg)
{
  int x = 0;
  int y = 0;
  break_obj *b = NULL;

  x = x0 - 25;
  y = y0 - 25;
  b = new break_obj(x, y, canvas(), id_arg);

  if(b != NULL)
    {
      breaks->append(b);
      qmain->setSaved(false);
    }
  else
    cerr << "Warning! Could not insert object into break list. "
	 << "File " << __FILE__ << ", "
	 << "line " << __LINE__ << "." << endl;

  updateLister();
}

/*
** -- drawContinue() --
*/

void editor::drawContinue(const int x0, const int y0, const int id_arg)
{
  int x = 0;
  int y = 0;
  continue_obj *c = NULL;

  x = x0 - 25;
  y = y0 - 25;
  c = new continue_obj(x, y, canvas(), id_arg);

  if(c != NULL)
    {
      continues->append(c);
      qmain->setSaved(false);
    }
  else
    cerr << "Warning! Could not insert object into continue list. "
	 << "File " << __FILE__ << ", "
	 << "line " << __LINE__ << "." << endl;

  updateLister();
}

/*
** -- drawReturn() --
*/

void editor::drawReturn(const int x0, const int y0, const int id_arg)
{
  int x = 0;
  int y = 0;
  return_obj *r = NULL;

  x = x0 - 25;
  y = y0 - 25;
  r = new return_obj(x, y, canvas(), id_arg);

  if(r != NULL)
    {
      returns->append(r);
      qmain->setSaved(false);
    }
  else
    cerr << "Warning! Could not insert object into return list. "
	 << "File " << __FILE__ << ", "
	 << "line " << __LINE__ << "." << endl;

  updateLister();
}

/*
** -- drawConstant() --
*/

void editor::drawConstant(const int x0, const int y0, const int type,
			  const int id_arg, const int value_arg)
{
  int x = 0;
  int y = 0;
  constant *c = NULL;

  x = x0 - 25;
  y = y0 - 25;

  if(type == CONSTANT)
    c = new constant(x, y, canvas(), type, this, id_arg, value_arg);

  if(c != NULL)
    {
      constants->append(c);
      qmain->setSaved(false);
    }
  else
    cerr << "Warning! Could not insert object into constant list. "
	 << "File " << __FILE__ << ", "
	 << "line " << __LINE__ << "." << endl;

  updateLister();
}

/*
** -- drawFunction() --
*/

void editor::drawFunction(const int x0, const int y0, const int type,
			  const int cwidth, const int cheight)
{
  int x = 0;
  int y = 0;
  function *f = NULL;

  x = x0 - 25;
  y = y0 - 25;

  if(type == FUNCTION)
    f = new function(x, y, canvas(), type, cwidth, cheight);

  if(f != NULL)
    {
      functions->append(f);
      qmain->setSaved(false);
    }
  else
    cerr << "Warning! Could not insert object into function list. "
	 << "File " << __FILE__ << ", "
	 << "line " << __LINE__ << "." << endl;

  updateLister();
}

/*
** -- updateFuncSubmenu() --
*/

void editor::updateFuncSubmenu(void)
{
  unsigned int i = 0;

  func_submenu->clear();
  qHeapSort(qmain->getUsedFunctionNames());

  for(i = 0; i < (unsigned int) qmain->getUsedFunctionNames().size(); i++)
    func_submenu->insertItem(qmain->getUsedFunctionNames()[i],
			     this, SLOT(slotAddFunction(void)));

  // qmain->setSaved(false);
}

/*
** -- updateTaskSubmenu() --
*/

void editor::updateTaskSubmenu(void)
{
  unsigned int i = 0;

  task_submenu->clear();
  qHeapSort(qmain->getUsedTaskNames());

  for(i = 0; i < (unsigned int) qmain->getUsedTaskNames().size(); i++)
    task_submenu->insertItem(qmain->getUsedTaskNames()[i],
			     this, SLOT(slotAddTask(void)));

  // qmain->setSaved(false);
}

/*
** -- updateSubroutineSubmenu() --
*/

void editor::updateSubroutineSubmenu(void)
{
  unsigned int i = 0;

  subroutine_submenu->clear();
  qHeapSort(qmain->getUsedSubroutineNames());

  for(i = 0; i < (unsigned int) qmain->getUsedSubroutineNames().size(); i++)
    subroutine_submenu->insertItem(qmain->getUsedSubroutineNames()[i],
				   this, SLOT(slotAddSubroutine(void)));

  // qmain->setSaved(false);
}

/*
** -- drawVariable() --
*/

void editor::drawVariable(const int x0, const int y0, const int type,
			  const bool is_global_arg, const int obj_id_arg,
			  const QString &copy_name)
{
  int x = 0;
  int y = 0;
  QString str = "";
  variable *v = NULL;

  x = x0 - 25;
  y = y0 - 25;

  if(type == VARIABLE)
    v = new variable(x, y, canvas(), type, used_variable_names, this,
		     isMainEditor() || is_global_arg, obj_id_arg,
		     copy_name);

  if(v != NULL)
    {
      str = v->getName();

      if(copy_name.isEmpty())
	used_variable_names.append(str);

      variables->append(v);
      qmain->setSaved(false);
    }
  else
    cerr << "Warning! Could not insert object into variable list. "
	 << "File " << __FILE__ << ", "
	 << "line " << __LINE__ << "." << endl;

  updateLister();
}

/*
** -- drawLoop() --
*/

void editor::drawLoop(const int x0, const int y0, const int type,
		      QPtrList<file_class> *list, const int id_arg,
		      const QString &condition_str, const int cwidth,
		      const int cheight)
{
  int x = 0;
  int y = 0;
  loop *l = NULL;

  x = x0 - 25;
  y = y0 - 25;

  if(type == FOR_LOOP)
    l = new loop(x, y, "i", "N", canvas(), type, "For Loop", list,
		 id_arg, condition_str, cwidth, cheight);
  else if(type == WHILE_LOOP)
    l = new loop(x, y, "0", "T", canvas(), type, "While Loop", list, id_arg,
		 condition_str, cwidth, cheight);
  else if(type == DO_WHILE_LOOP)
    l = new loop(x, y, "1", "T", canvas(), type, "Do While Loop", list,
		 id_arg, condition_str, cwidth, cheight);
  else if(type == REPEAT_LOOP)
    l = new loop(x, y, "?", "N", canvas(), type, "Repeat Loop", list,
		 id_arg, condition_str, cwidth, cheight);

  if(l != NULL)
    {
      loops->append(l);
      qmain->setSaved(false);
    }
  else
    cerr << "Warning! Could not insert object into loop list. "
	 << "File " << __FILE__ << ", "
	 << "line " << __LINE__ << "." << endl;

  updateLister();
}

/*
** -- keyPressEvent() --
*/

void editor::keyPressEvent(QKeyEvent *e)
{
  int distance = 32;
  bool moved = false;
  char buffer[256];
  QString str = "";
  QStatusBar *status_bar = ((QMainWindow *) dialog_parent)->statusBar();

  if(e == NULL)
    return;
  else if(selected_item == NULL)
    return;

  switch(e->key())
    {
    case Qt::Key_Up:
      {
	if(tw->getType() == WIRE)
	  return;

	if(!(e->state() & Qt::ShiftButton))
	  distance = 1;

	if(collisionDetected(selected_item, QPoint
			     ((int) selection->x() +
			      selection->width() / 2,
			      (int) selection->y() - distance)))
	  return;

	if(selected_item_type == BREAK)
	  moved = breaks->at(breaks->find((break_obj *) selected_item))->move
	    (editor::MOVE_UP, distance);
	else if(selected_item_type == CONTINUE)
	  moved = continues->at
	    (continues->find((continue_obj *) selected_item))->move
	    (editor::MOVE_UP, distance);
	else if(selected_item_type == RETURN)
	  moved = returns->at
	    (returns->find((return_obj *) selected_item))->move
	    (editor::MOVE_UP, distance);
	else if(selected_item_type == GATE)
	  moved = gates->at(gates->find((gate *) selected_item))->move
	    (editor::MOVE_UP, distance);
	else if(selected_item_type == LOOP)
	  moved = loops->at(loops->find((loop *) selected_item))->move
	    (editor::MOVE_UP, distance);
	else if(selected_item_type == CONSTANT)
	  moved = constants->at(constants->
				find((constant *) selected_item))->move
	    (editor::MOVE_UP, distance);
	else if(selected_item_type == VARIABLE)
	  moved = variables->at(variables->
				find((variable *) selected_item))->move
	    (editor::MOVE_UP, distance);
	else if(selected_item_type == TASK)
	  moved = tasks->at(tasks->find((task *) selected_item))->move
	    (editor::MOVE_UP, distance);
	else if(selected_item_type == SUBROUTINE)
	  moved = subroutines->at
	    (subroutines->find((subroutine *) selected_item))->move
	    (editor::MOVE_UP, distance);
	else if(selected_item_type == FUNCTION)
	  moved = functions->at(functions->
				find((function *) selected_item))->move
	    (editor::MOVE_UP, distance);

	if(moved)
	  {
	    selection->move(selection->x(), selection->y() - distance);
	    (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
			    selected_item->getX(),
			    selected_item->getY());
	    status_bar->message(buffer);
	    canvas()->update();
	    qmain->setSaved(false);
	  }

	updateLister();
	break;
      }
    case Qt::Key_Down:
      {
	if(tw->getType() == WIRE)
	  return;

	if(!(e->state() & Qt::ShiftButton))
	  distance = 1;

	if(collisionDetected(selected_item, QPoint
			     ((int) selection->x() +
			      selection->width() / 2,
			      (int) selection->y() + distance +
			      selection->height())))
	  return;

	if(selected_item_type == BREAK)
	  moved = breaks->at(breaks->find((break_obj *) selected_item))->move
	    (editor::MOVE_DOWN, distance);
	else if(selected_item_type == CONTINUE)
	  moved = continues->at
	    (continues->find((continue_obj *) selected_item))->move
	    (editor::MOVE_DOWN, distance);
	else if(selected_item_type == RETURN)
	  moved = returns->at
	    (returns->find((return_obj *) selected_item))->move
	    (editor::MOVE_DOWN, distance);
	else if(selected_item_type == GATE)
	  moved = gates->at(gates->find((gate *) selected_item))->move
	    (editor::MOVE_DOWN, distance);
	else if(selected_item_type == LOOP)
	  moved = loops->at(loops->find((loop *) selected_item))->move
	    (editor::MOVE_DOWN, distance);
	else if(selected_item_type == CONSTANT)
	  moved = constants->at(constants->
				find((constant *) selected_item))->move
	    (editor::MOVE_DOWN, distance);
	else if(selected_item_type == VARIABLE)
	  moved = variables->at(variables->
				find((variable *) selected_item))->move
	    (editor::MOVE_DOWN, distance);
	else if(selected_item_type == TASK)
	  moved = tasks->at(tasks->find((task *) selected_item))->move
	    (editor::MOVE_DOWN, distance);
	else if(selected_item_type == SUBROUTINE)
	  moved = subroutines->at
	    (subroutines->find((subroutine *) selected_item))->move
	    (editor::MOVE_DOWN, distance);
	else if(selected_item_type == FUNCTION)
	  moved = functions->at(functions->
				find((function *) selected_item))->move
	    (editor::MOVE_DOWN, distance);

	if(moved)
	  {
	    selection->move(selection->x(), selection->y() + distance);
	    (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
			    selected_item->getX(),
			    selected_item->getY());
	    status_bar->message(buffer);
	    canvas()->update();
	    qmain->setSaved(false);
	  }

	updateLister();
	break;
      }
    case Qt::Key_Left:
      {
	if(tw->getType() == WIRE)
	  return;

	if(!(e->state() & Qt::ShiftButton))
	  distance = 1;

	if(collisionDetected(selected_item, QPoint
			     ((int) selection->x() - distance,
			      (int) selection->y() +
			      selection->height() / 2)))
	  return;

	if(selected_item_type == BREAK)
	  moved = breaks->at(breaks->find((break_obj *) selected_item))->move
	    (editor::MOVE_LEFT, distance);
	else if(selected_item_type == CONTINUE)
	  moved = continues->at
	    (continues->find((continue_obj *) selected_item))->move
	    (editor::MOVE_LEFT, distance);
	else if(selected_item_type == RETURN)
	  moved = returns->at
	    (returns->find((return_obj *) selected_item))->move
	    (editor::MOVE_LEFT, distance);
	else if(selected_item_type == GATE)
	  moved = gates->at(gates->find((gate *) selected_item))->move
	    (editor::MOVE_LEFT, distance);
	else if(selected_item_type == LOOP)
	  moved = loops->at(loops->find((loop *) selected_item))->move
	    (editor::MOVE_LEFT, distance);
	else if(selected_item_type == CONSTANT)
	  moved = constants->at(constants->
				find((constant *) selected_item))->
	    move(editor::MOVE_LEFT, distance);
	else if(selected_item_type == VARIABLE)
	  moved = variables->at(variables->
				find((variable *) selected_item))->move
	    (editor::MOVE_LEFT, distance);
	else if(selected_item_type == TASK)
	  moved = tasks->at(tasks->find((task *) selected_item))->move
	    (editor::MOVE_LEFT, distance);
	else if(selected_item_type == SUBROUTINE)
	  moved = subroutines->at
	    (subroutines->find((subroutine *) selected_item))->move
	    (editor::MOVE_LEFT, distance);
	else if(selected_item_type == FUNCTION)
	  moved = functions->at(functions->
				find((function *) selected_item))->move
	    (editor::MOVE_LEFT, distance);

	if(moved)
	  {
	    selection->move(selection->x() - distance, selection->y());
	    (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
			    selected_item->getX(),
			    selected_item->getY());
	    status_bar->message(buffer);
	    canvas()->update();
	    qmain->setSaved(false);
	  }

	updateLister();
	break;
      }
    case Qt::Key_Right:
      {
	if(tw->getType() == WIRE)
	  return;

	if(!(e->state() & Qt::ShiftButton))
	  distance = 1;

	if(collisionDetected(selected_item, QPoint
			     ((int) selection->x() + distance +
			      selection->width(),
			      (int) selection->y() +
			      selection->height() / 2)))
	  return;

	if(selected_item_type == BREAK)
	  moved = breaks->at(breaks->find((break_obj *) selected_item))->move
	    (editor::MOVE_RIGHT, distance);
	else if(selected_item_type == CONTINUE)
	  moved = continues->at
	    (continues->find((continue_obj *) selected_item))->move
	    (editor::MOVE_RIGHT, distance);
	else if(selected_item_type == RETURN)
	  moved = returns->at
	    (returns->find((return_obj *) selected_item))->move
	    (editor::MOVE_RIGHT, distance);
	else if(selected_item_type == GATE)
	  moved = gates->at(gates->find((gate *) selected_item))->move
	    (editor::MOVE_RIGHT, distance);
	else if(selected_item_type == LOOP)
	  moved = loops->at(loops->find((loop *) selected_item))->move
	    (editor::MOVE_RIGHT, distance);
	else if(selected_item_type == CONSTANT)
	  moved = constants->at(constants->
				find((constant *) selected_item))->move
	    (editor::MOVE_RIGHT, distance);
	else if(selected_item_type == VARIABLE)
	  moved = variables->at(variables->
				find((variable *) selected_item))->move
	    (editor::MOVE_RIGHT, distance);
	else if(selected_item_type == TASK)
	  moved = tasks->at(tasks->find((task *) selected_item))->move
	    (editor::MOVE_RIGHT, distance);
	else if(selected_item_type == SUBROUTINE)
	  moved = subroutines->at
	    (subroutines->find((subroutine *) selected_item))->move
	    (editor::MOVE_RIGHT, distance);
	else if(selected_item_type == FUNCTION)
	  moved = functions->at(functions->
				find((function *) selected_item))->move
	    (editor::MOVE_RIGHT, distance);

	if(moved)
	  {
	    selection->move(selection->x() + distance, selection->y());
	    (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
			    selected_item->getX(),
			    selected_item->getY());
	    status_bar->message(buffer);
	    canvas()->update();
	    qmain->setSaved(false);
	  }

	updateLister();
	break;
      }
    case Qt::Key_Delete: case Qt::Key_Backspace: case 0:
      {
	/*
	** Was the keyboard actually pressed?
	*/

	if(e->key() != 0)
	  if(!askAboutDelete())
	    return;

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	if(selected_item_type == BREAK)
	  {
	    if(!breaks->isEmpty())
	      {
		breaks->remove((break_obj *) selected_item);
		selection->hide();
		selected_item_type = -1;
		selected_item = NULL;

		if(sel_timer != NULL && sel_timer->isActive())
		  sel_timer->stop();

		canvas()->update();
		qmain->setSaved(false);
		status_bar->clear();
	      }
	  }
	else if(selected_item_type == CONTINUE)
	  {
	    if(!continues->isEmpty())
	      {
		continues->remove((continue_obj *) selected_item);
		selection->hide();
		selected_item_type = -1;
		selected_item = NULL;

		if(sel_timer != NULL && sel_timer->isActive())
		  sel_timer->stop();

		canvas()->update();
		qmain->setSaved(false);
		status_bar->clear();
	      }
	  }
	else if(selected_item_type == RETURN)
	  {
	    if(!returns->isEmpty())
	      {
		returns->remove((return_obj *) selected_item);
		selection->hide();
		selected_item_type = -1;
		selected_item = NULL;

		if(sel_timer != NULL && sel_timer->isActive())
		  sel_timer->stop();

		canvas()->update();
		qmain->setSaved(false);
		status_bar->clear();
	      }
	  }
	else if(selected_item_type == GATE)
	  {
	    if(!gates->isEmpty())
	      {
		gates->remove((gate *) selected_item);
		selection->hide();
		selected_item_type = -1;
		selected_item = NULL;

		if(sel_timer != NULL && sel_timer->isActive())
		  sel_timer->stop();

		canvas()->update();
		qmain->setSaved(false);
		status_bar->clear();
	      }
	  }
	else if(selected_item_type == LOOP)
	  {
	    if(!loops->isEmpty())
	      {
		loops->remove((loop *) selected_item);
		selection->hide();
		selected_item_type = -1;
		selected_item = NULL;

		if(sel_timer != NULL && sel_timer->isActive())
		  sel_timer->stop();

		canvas()->update();
		qmain->setSaved(false);
		status_bar->clear();
	      }
	  }
	else if(selected_item_type == CONSTANT)
	  {
	    if(!constants->isEmpty())
	      {
		constants->remove((constant *) selected_item);
		selection->hide();
		selected_item_type = -1;
		selected_item = NULL;

		if(sel_timer != NULL && sel_timer->isActive())
		  sel_timer->stop();

		canvas()->update();
		qmain->setSaved(false);
		status_bar->clear();
	      }
	  }
	else if(selected_item_type == VARIABLE)
	  {
	    if(!variables->isEmpty())
	      {
		str = ((variable *) selected_item)->getName();
		used_variable_names.remove(str);
		variables->remove((variable *) selected_item);
		selection->hide();
		selected_item_type = -1;
		selected_item = NULL;

		if(sel_timer != NULL && sel_timer->isActive())
		  sel_timer->stop();

		canvas()->update();
		qmain->setSaved(false);
		status_bar->clear();
	      }
	  }
	else if(selected_item_type == TASK)
	  {
	    if(!tasks->isEmpty())
	      {
		tasks->remove((task *) selected_item);
		selection->hide();
		selected_item_type = -1;
		selected_item = NULL;

		if(sel_timer != NULL && sel_timer->isActive())
		  sel_timer->stop();

		canvas()->update();
		qmain->setSaved(false);
		status_bar->clear();
	      }
	  }
	else if(selected_item_type == SUBROUTINE)
	  {
	    if(!subroutines->isEmpty())
	      {
		subroutines->remove((subroutine *) selected_item);
		selection->hide();
		selected_item_type = -1;
		selected_item = NULL;

		if(sel_timer != NULL && sel_timer->isActive())
		  sel_timer->stop();

		canvas()->update();
		qmain->setSaved(false);
		status_bar->clear();
	      }
	  }
	else if(selected_item_type == FUNCTION)
	  {
	    if(!functions->isEmpty())
	      {
		functions->remove((function *) selected_item);
		selection->hide();
		selected_item_type = -1;
		selected_item = NULL;

		if(sel_timer != NULL && sel_timer->isActive())
		  sel_timer->stop();

		canvas()->update();
		qmain->setSaved(false);
		status_bar->clear();
	      }
	  }

	updateLister();
	QApplication::restoreOverrideCursor();
	break;
      }
    default:
      break;
    }
}

/*
** -- slotEdit() --
*/

void editor::slotEdit(void)
{
  QPoint pt;

  if(selected_item != NULL)
    if(selected_item_type == FUNCTION || selected_item_type == TASK ||
       selected_item_type == LOOP || selected_item_type == CONSTANT ||
       selected_item_type == VARIABLE || selected_item_type == SUBROUTINE)
      {
	if(!isMainEditor())
	  if(parentWidget()->parentWidget() == NULL)
	    return;

	pt.setX(parentWidget()->pos().x() + parentWidget()->width() / 2);
	pt.setY(parentWidget()->pos().y() + parentWidget()->height() / 2);
	selected_item->setParent(parentWidget()->parentWidget(), pt);

	if(selected_item_type == FUNCTION)
	  ((function *) selected_item)->show();
	else if(selected_item_type == TASK)
	  ((task *) selected_item)->show();
	else if(selected_item_type == SUBROUTINE)
	  ((subroutine *) selected_item)->show();
	else if(selected_item_type == LOOP)
	  ((loop *) selected_item)->show();
	else if(selected_item_type == CONSTANT)
	  {
	    selected_item->setValue(((constant *) selected_item)->getValue());
	    ((constant *) selected_item)->show();
	  }
	else if(selected_item_type == VARIABLE)
	  slotRename();
      }
}

/*
** -- slotRename() --
*/

void editor::slotRename(void)
{
  QPoint pt;

  if(selected_item_type == FUNCTION)
    rw->setCaption("Function Rename");
  else if(selected_item_type == TASK)
    rw->setCaption("Task Rename");
  else if(selected_item_type == SUBROUTINE)
    rw->setCaption("Subroutine Rename");
  else if(selected_item_type == VARIABLE)
    rw->setCaption("Variable Rename");
  else
    rw->setCaption("Rename");

  if(selected_item != NULL)
    rw->setNameStr(selected_item->getName());

  pt.setX(parentWidget()->pos().x() + parentWidget()->width() / 2);
  pt.setY(parentWidget()->pos().y() + parentWidget()->height() / 2);
  rw->setObject(selected_item);
  rw->move(pt);
  rw->exec();

  /*
  ** Update the Canvas Items panel.
  */

  updateLister();

  /*
  ** Set the save state to false.
  */

  qmain->setSaved(false);
}

/*
** -- setMain() --
*/

void editor::setMain(const bool is_main_editor_arg)
{
  is_main_editor = is_main_editor_arg;
}

/*
** -- isMainEditor() --
*/

bool editor::isMainEditor(void)
{
  return is_main_editor;
}

void editor::updateLister(void)
{
  if(lister->isVisible())
    lister->fill();
}

/*
** -- slotListAll() --
*/

void editor::slotListAll(void)
{
  lister->fill();
  lister->show();
}

/*
** -- slotAddFunction() --
*/

void editor::slotAddFunction(void)
{
  /*
  ** Create only a copy.
  */

  // drawFunction(last_pos.x(), last_pos.y(), FUNCTION);
  // canvas()->update();
}

/*
** -- slotAddTask() --
*/

void editor::slotAddTask(void)
{
}

/*
** -- slotAddSubroutine() --
*/

void editor::slotAddSubroutine(void)
{
}

/*
** -- deleteAt() --
*/

void editor::deleteSelection(const int x, const int y)
{
  QKeyEvent ek(QEvent::KeyPress, 0, 127, 0); /*
					     ** The second argument (0) is used for
					     ** deciding whether or not to call
					     ** askAboutDelete() in keyPressEvent().
					     */
  QMouseEvent *em = NULL;
  QPoint pt;

  pt.setX(x);
  pt.setY(y);
  em = new QMouseEvent(QEvent::MouseButtonPress, pt,
		       Qt::LeftButton, Qt::LeftButton);

  if(em != NULL)
    {
      contentsMousePressEvent(em);
      keyPressEvent(&ek);
    }

  delete em;
}

/*
** -- slotDelete() --
*/

void editor::slotDelete(void)
{
  QKeyEvent e(QEvent::KeyPress, 0, 127, 0); /*
					    ** The second argument (0) is used for
					    ** deciding whether or not to call
					    ** askAboutDelete() in keyPressEvent().
					    */

  if(askAboutDelete())
    keyPressEvent(&e);
}

/*
** -- dragMoveEvent() --
*/

void editor::dragMoveEvent(QDragMoveEvent *e)
{
  char buffer[32];
  gate *g = NULL;
  loop *l = NULL;
  task *t = NULL;
  QPoint pt;
  constant *c = NULL;
  function *f = NULL;
  variable *v = NULL;
  break_obj *b = NULL;
  QStatusBar *status_bar = ((QMainWindow *) dialog_parent)->statusBar();
  return_obj *r_o = NULL;
  subroutine *s = NULL;
  continue_obj *c_o = NULL;

  if(e == NULL)
    return;

  if(selected_item == NULL)
    return;

  pt = e->pos();
  pt += QPoint(horizontalScrollBar()->value(),
	       verticalScrollBar()->value());

  if(collisionDetected(selected_item, pt))
    {
      e->ignore();
      return;
    }
  else
    e->accept();

  if(selected_item_type == GATE)
    {
      g = (gate *) selected_item;
      g->move(pt);
      selection->move(pt.x() - 5 - 15, pt.y() - 5 - 25);
      selection->show();
    }
  else if(selected_item_type == LOOP)
    {
      l = (loop *) selected_item;
      l->move(pt);
      selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
      selection->show();
    }
  else if(selected_item_type == BREAK)
    {
      b = (break_obj *) selected_item;
      b->move(pt);
      selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
      selection->show();
    }
  else if(selected_item_type == CONTINUE)
    {
      c_o = (continue_obj *) selected_item;
      c_o->move(pt);
      selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
      selection->show();
    }
  else if(selected_item_type == RETURN)
    {
      r_o = (return_obj *) selected_item;
      r_o->move(pt);
      selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
      selection->show();
    }
  else if(selected_item_type == CONSTANT)
    {
      c = (constant *) selected_item;
      c->move(pt);
      selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
      selection->show();
    }
  else if(selected_item_type == VARIABLE)
    {
      v = (variable *) selected_item;
      v->move(pt);
      selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
      selection->show();
    }
  else if(selected_item_type == TASK)
    {
      t = (task *) selected_item;
      t->move(pt);
      selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
      selection->show();
    }
  else if(selected_item_type == SUBROUTINE)
    {
      s = (subroutine *) selected_item;
      s->move(pt);
      selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
      selection->show();
    }
  else if(selected_item_type == FUNCTION)
    {
      f = (function *) selected_item;
      f->move(pt);
      selection->move(pt.x() - 5 - 25, pt.y() - 5 - 25);
      selection->show();
    }

  canvas()->update();
  qmain->setSaved(false);

  if(status_bar != NULL)
    {
      (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
		      selected_item->getX(), selected_item->getY());
      status_bar->message(buffer);
      updateLister();
    }

  canvas()->update();
}

/*
** -- slotShowHelp() --
*/

void editor::slotShowHelp(void)
{
}

/*
** -- slotShowMain() --
*/

void editor::slotShowMain(void)
{
  qmain->showMain();
}

/*
** -- slotSelTimerTimeout() --
*/

void editor::slotSelTimerTimeout(void)
{
  if(tw->getType() != WIRE && sel_timer != NULL)
    {
      sel_timer->stop();

      if(selected_item != NULL)
	selection->show();

      canvas()->update();
    }
  else if(selected_item != NULL)
    {
      if(selection->isVisible())
	selection->hide();
      else
	selection->show();

      canvas()->update();
    }
}

/*
** -- collisionDetected() --
*/

bool editor::collisionDetected(object *item_arg, const QPoint &pt)
{
  QRect rec;
  unsigned int i = 0;
  QCanvasItemList hits;

  rec.setTopLeft(QPoint(pt.x() - 50, pt.y() - 50));
  rec.setBottomRight(QPoint(pt.x() + 50, pt.y() + 50));
  hits = canvas()->collisions(rec);

  for(i = 0; i < (unsigned int) constants->count(); i++)
    if(hits.contains(constants->at(i)->getPolygon()))
      if((constant *) item_arg != constants->at(i))
	{
	  hits.clear();
	  return true;
	}

  for(i = 0; i < (unsigned int) functions->count(); i++)
    if(hits.contains(functions->at(i)->getPolygon()))
      if((function *) item_arg != functions->at(i))
	{
	  hits.clear();
	  return true;
	}

  for(i = 0; i < (unsigned int) gates->count(); i++)
    if(hits.contains(gates->at(i)->getPolygon()))
      if((gate *) item_arg != gates->at(i))
	{
	  hits.clear();
	  return true;
	}

  for(i = 0; i < (unsigned int) breaks->count(); i++)
    if(hits.contains(breaks->at(i)->getPolygon()))
      if((break_obj *) item_arg != breaks->at(i))
	{
	  hits.clear();
	  return true;
	}

  for(i = 0; i < (unsigned int) continues->count(); i++)
    if(hits.contains(continues->at(i)->getPolygon()))
      if((continue_obj *) item_arg != continues->at(i))
	{
	  hits.clear();
	  return true;
	}

  for(i = 0; i < (unsigned int) returns->count(); i++)
    if(hits.contains(returns->at(i)->getPolygon()))
      if((return_obj *) item_arg != returns->at(i))
	{
	  hits.clear();
	  return true;
	}

  for(i = 0; i < (unsigned int) loops->count(); i++)
    if(hits.contains(loops->at(i)->getPolygon()))
      if((loop *) item_arg != loops->at(i))
	{
	  hits.clear();
	  return true;
	}

  for(i = 0; i < (unsigned int) tasks->count(); i++)
    if(hits.contains(tasks->at(i)->getPolygon()))
      if((task *) item_arg != tasks->at(i))
	{
	  hits.clear();
	  return true;
	}

  for(i = 0; i < (unsigned int) subroutines->count(); i++)
    if(hits.contains(subroutines->at(i)->getPolygon()))
      if((subroutine *) item_arg != subroutines->at(i))
	{
	  hits.clear();
	  return true;
	}

  for(i = 0; i < (unsigned int) variables->count(); i++)
    if(hits.contains(variables->at(i)->getPolygon()))
      if((variable *) item_arg != variables->at(i))
	{
	  hits.clear();
	  return true;
	}

  return false;
}

/*
** -- enlarge() --
*/

void editor::enlarge(void)
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  canvas()->resize(canvas()->width() * 4/3, canvas()->height() * 4/3);
  qmain->setSaved(false);
  QApplication::restoreOverrideCursor();
}

/*
** -- slotShowFuncs() --
*/

void editor::slotShowFuncs(void)
{
  if(fw->getParent()->isVisible())
    fw->getParent()->raise();
  else
    {
      fw->hideDialog();
      fw->displayDialog();
    }
}

/*
** -- slotShowTools() --
*/

void editor::slotShowTools(void)
{
  if(tw->getParent()->isVisible())
    tw->getParent()->raise();
  else
    {
      tw->hideDialog();
      tw->displayDialog();
    }
}

/*
** -- updateVariableNamesList() --
*/

void editor::updateVariableNamesList(const QString &str)
{
  if(isMainEditor())
    {
      used_variable_names.remove(str);
      used_variable_names.append(str);
    }
  else
    used_variable_names.remove(str);
}

/*
** -- scrollTo() --
*/

void editor::scrollTo(const int x, const int y)
{
  QMouseEvent *e = NULL;
  QPoint pt;

  pt.setX(x);
  pt.setY(y);
  setContentsPos(x - parentWidget()->width() / 2,
		 y - parentWidget()->height() / 2);
  e = new QMouseEvent(QEvent::MouseButtonPress, pt,
		      Qt::LeftButton, Qt::LeftButton);

  if(e != NULL)
    contentsMousePressEvent(e);

  delete e;
}

/*
** -- writeToFile() --
*/

void editor::writeToFile(QTextStream &qts, QString &error)
{
  unsigned int i = 0;

  if(isMainEditor())
    {
      qts << "<max-id>" << qtnqc_main::current_id << endl;
      qts << "<canvas-width>" << canvas()->width() << endl;
      qts << "<canvas-height>" << canvas()->height() << endl;
    }

  for(i = 0; i < (unsigned int) loops->count(); i++)
    {
      loops->at(i)->writeToFile(qts, error, parent_id);

      if(!error.isEmpty())
	return;
    }

  for(i = 0; i < (unsigned int) variables->count(); i++)
    {
      variables->at(i)->writeToFile(qts, error, parent_id);

      if(!error.isEmpty())
	return;
    }

  for(i = 0; i < (unsigned int) constants->count(); i++)
    {
      constants->at(i)->writeToFile(qts, error, parent_id);

      if(!error.isEmpty())
	return;
    }

  for(i = 0; i < (unsigned int) gates->count(); i++)
    {
      gates->at(i)->writeToFile(qts, error, parent_id);

      if(!error.isEmpty())
	return;
    }

  for(i = 0; i < (unsigned int) breaks->count(); i++)
    {
      breaks->at(i)->writeToFile(qts, error, parent_id);

      if(!error.isEmpty())
	return;
    }

  for(i = 0; i < (unsigned int) continues->count(); i++)
    {
      continues->at(i)->writeToFile(qts, error, parent_id);

      if(!error.isEmpty())
	return;
    }

  for(i = 0; i < (unsigned int) returns->count(); i++)
    {
      returns->at(i)->writeToFile(qts, error, parent_id);

      if(!error.isEmpty())
	return;
    }

  for(i = 0; i < (unsigned int) tasks->count(); i++)
    {
      tasks->at(i)->writeToFile(qts, error, parent_id);

      if(!error.isEmpty())
	return;
    }

  for(i = 0; i < (unsigned int) subroutines->count(); i++)
    {
      subroutines->at(i)->writeToFile(qts, error, parent_id);

      if(!error.isEmpty())
	return;
    }

  for(i = 0; i < (unsigned int) functions->count(); i++)
    {
      functions->at(i)->writeToFile(qts, error, parent_id);

      if(!error.isEmpty())
	return;
    }
}

/*
** -- readFile() --
*/

void editor::readFile(QFile &qf)
{
  int x = -1;
  int y = -1;
  int canvas_width = 0;
  int canvas_height = 0;
  int mcanvas_width = 0;
  int mcanvas_height = 0;
  int type = INT_MIN;
  int value = 0;
  bool is_global = false;
  long id = LONG_MIN; /* Object Id */
  long pid = LONG_MIN; /* Parent Id */
  QString str = "";
  QString f_t_name = "";
  QString condition_str = "";
  file_class *fc = NULL;
  QTextStream qts;
  unsigned int i = 0;
  QProgressDialog progress(qmain->getMainWindow());

  progress.setLabelText("Reading file...");
  progress.setCaption("Reading File");
  progress.update();
  qts.setDevice(&qf);
  fc_list.setAutoDelete(true);

  while(!qts.eof())
    {
      id = pid = LONG_MIN;
      x = y = type = -1;
      value = 0;
      f_t_name = "";
      str = qts.readLine().stripWhiteSpace();

      if(mcanvas_width == 0 && str.startsWith("<canvas-width>"))
	if((mcanvas_width = atoi(str.remove("<canvas-width>"))) < MAX_WIDTH)
	  mcanvas_width = MAX_WIDTH;

      if(mcanvas_height == 0 && str.startsWith("<canvas-height>"))
	if((mcanvas_height = atoi(str.remove("<canvas-height>"))) < MAX_HEIGHT)
	  mcanvas_height = MAX_HEIGHT;

      if(str == "<object>")
	{
	  qtnqc_main::nextId();

	  while(!qts.eof())
	    {
	      str = qts.readLine().stripWhiteSpace();

	      if(str == "</object>")
		break;
	      else if(str.startsWith("<type>"))
		type = atoi(str.remove("<type>"));
	      else if(str.startsWith("<x-coord>"))
		x = atoi(str.remove("<x-coord>"));
	      else if(str.startsWith("<y-coord>"))
		y = atoi(str.remove("<y-coord>"));
	      else if(str.startsWith("<id>"))
		id = atol(str.remove("<id>"));
	      else if(str.startsWith("<parent-id>"))
		pid = atol(str.remove("<parent-id>"));
	      else if(str.startsWith("<name>"))
		f_t_name = str.remove("<name>");
	      else if(str.startsWith("<value>"))
		value = atoi(str.remove("<value>"));
	      else if(str.startsWith("<condition_str>"))
		condition_str = str.remove("<condition_str>");
	      else if(str.startsWith("<canvas-width>"))
		canvas_width = atoi(str.remove("<canvas-width>"));
	      else if(str.startsWith("<canvas-height>"))
		canvas_height = atoi(str.remove("<canvas-height>"));
	      else if(str.startsWith("<is-global>"))
		is_global = (str.remove("<is-global>") == "1");
	    }

	  if(type >= 0 && id >= 0)
	    if((fc = new file_class()) != NULL)
	      {
		if(type == FOR_LOOP ||
		   type == REPEAT_LOOP ||
		   type == WHILE_LOOP ||
		   type == DO_WHILE_LOOP ||
		   type == TASK ||
		   type == SUBROUTINE ||
		   type == FUNCTION ||
		   type == VARIABLE ||
		   type == CONSTANT ||
		   type == BREAK ||
		   type == CONTINUE ||
		   type == RETURN)
		  fc->x = x + 25;
		else
		  fc->x = x + 15; /* Gates. */

		fc->y = y + 25;
		fc->id = id;
		fc->pid = pid;
		fc->type = type;
		fc->value = value;
		fc->name = f_t_name.left(5);
		fc->condition_str = condition_str;
		fc_list.append(fc);
		fc->canvas_width = canvas_width;
		fc->canvas_height = canvas_height;
		fc->is_global = is_global;
	      }
	}
    }

  progress.setCancelButton(NULL);
  progress.setTotalSteps(fc_list.count());
  progress.show();
  progress.update();
  canvas()->resize(mcanvas_width, mcanvas_height);

  /*
  ** Process pending events.
  */

  qapp->processEvents();

  for(i = 0; i < (unsigned int) fc_list.count(); i++)
    {
      progress.setProgress(i);
      progress.update();
      qapp->processEvents();

      if(fc_list.at(i)->pid == -1)
	{
	  fc = fc_list.at(i);
	  drawGeneric(fc, &fc_list);
	  fc_list.remove(i);
	  i -= 1;
	}
    }

  fc_list.clear();
  canvas()->update();
  qapp->processEvents();
}

/*
** -- createChildren() --
*/

void editor::createChildren(QPtrList<file_class> *list)
{
  unsigned int i = 0;

  if(list == NULL)
    return;

  for(i = 0; i < (unsigned int) list->count(); i++)
    if(parent_id == list->at(i)->pid)
      drawGeneric(list->at(i),
		  list);
}

/*
** -- drawGeneric() --
*/

void editor::drawGeneric(const file_class *fc,
			 QPtrList<file_class> *list)
{
  switch(fc->type)
    {
    case ADD_GATE:
    case DIVIDE_GATE:
    case MULTIPLY_GATE:
    case SUBTRACT_GATE:
    case NOT_GATE:
    case AND_GATE:
    case OR_GATE:
    case EQUALS_GATE:
    case GREATER_THAN_GATE:
    case LESS_THAN_GATE:
    case GREATER_THAN_OR_EQUAL_TO_GATE:
    case LESS_THAN_OR_EQUAL_TO_GATE:
    case NOT_EQUAL_TO_GATE:
    case BITWISE_AND_GATE:
    case BITWISE_XOR_GATE:
    case BITWISE_OR_GATE:
    case UNARY_MINUS_GATE:
    case BITWISE_NEGATION_GATE:
    case MODULO_GATE:
    case INCREMENT_GATE:
    case DECREMENT_GATE:
    case LEFT_SHIFT_GATE:
    case RIGHT_SHIFT_GATE:
    case ABSOLUTE_GATE:
    case SIGN_OF_GATE:
    case LESS_THAN_ZERO_GATE:
    case GREATER_THAN_ZERO_GATE:
    case GREATER_THAN_ONE_GATE:
      {
	drawGate(fc->x, fc->y, fc->type, fc->id);
	break;
      }
    case FOR_LOOP:
    case WHILE_LOOP:
    case DO_WHILE_LOOP:
    case REPEAT_LOOP:
      {
	drawLoop(fc->x, fc->y, fc->type, list, fc->id, fc->condition_str,
		 fc->canvas_width, fc->canvas_height);
	break;
      }
    case CONSTANT:
      {
	drawConstant(fc->x, fc->y, fc->type, fc->id, fc->value);
	break;
      }
    case VARIABLE:
      {
	drawVariable(fc->x, fc->y, fc->type, fc->is_global, fc->id,
		     fc->name);
	break;
      }
    case BREAK:
      {
	drawBreak(fc->x, fc->y, fc->id);
	break;
      }
    case CONTINUE:
      {
	drawContinue(fc->x, fc->y, fc->id);
	break;
      }
    case RETURN:
      {
	drawReturn(fc->x, fc->y, fc->id);
	break;
      }
    case TASK:
      {
	if(isMainEditor() && tasks->count() < MAX_TASKS)
	  drawTask(fc->x, fc->y, fc->type, list, fc->id, fc->name,
		   fc->canvas_width, fc->canvas_height);

	break;
      }
    case SUBROUTINE:
      {
	if(isMainEditor() && subroutines->count() < MAX_SUBROUTINES)
	  drawSubroutine(fc->x, fc->y, fc->type, list, fc->id, fc->name,
			 fc->canvas_width, fc->canvas_height);

	break;
      }
    case FUNCTION:
      {
	if(isMainEditor() && functions->count() < MAX_FUNCTIONS)
	  drawFunction(fc->x, fc->y, fc->type, fc->canvas_width,
		       fc->canvas_height);

	break;
      }
    default:
      break;
    }
}

/*
** -- setGlobal() --
*/

void editor::setGlobal(void)
{
  if(selected_item != NULL && selected_item_type == VARIABLE)
    {
      ((variable *) selected_item)->setAsGlobal();
      qmain->setSaved(false);
    }
}

/*
** -- slotCopyObject() --
*/

void editor::slotCopyObject(void)
{
  if(copied_item != NULL)
    {
      delete copied_item;
      copied_item = NULL;
      copied_item_type = -1;
    }

  if(selected_item != NULL && selected_item_type > -1)
    {
      /*
      ** Must create a "new" copied_item object.
      */

      if((copied_item = new object(selected_item->getType(),
				   selected_item->getX(),
				   selected_item->getY(),
				   selected_item->getParentCanvas(),
				   selected_item->getTypeName(),
				   NULL,
				   -1)) != NULL)
	{
	  copied_item_type = selected_item_type;
	  editmenu->setItemEnabled(editmenu->idAt(19), true);
	}
      else
	{
	  /*
	  ** Disable the paste menu.
	  */

	  copied_item_type = -1;
	  editmenu->setItemEnabled(editmenu->idAt(19), false);
	  cerr << "Warning! Could not copy selected item." << endl;
	}
    }
}

/*
** -- slotPasteObject() --
*/

void editor::slotPasteObject(void)
{
  QStatusBar *status_bar = ((QMainWindow *) dialog_parent)->statusBar();

  if(copied_item == NULL || copied_item_type == -1)
    return;

  /*
  ** Functions, Subroutines, and Tasks are not allowed to be copied.
  */

  switch(copied_item_type)
    {
    case GATE:
      {
	if(collisionDetected(copied_item, last_pos))
	  status_bar->message("Invalid paste location.", 3000);
	else
	  drawGate(last_pos.x(),
		   last_pos.y(),
		   ((gate *) copied_item)->getType(),
		   copied_item->getId());

	break;
      }
    case LOOP:
      {
	break;
      }
    case CONSTANT:
      {
	if(collisionDetected(copied_item, last_pos))
	  status_bar->message("Invalid paste location.", 3000);
	else
	  drawConstant(last_pos.x(),
		       last_pos.y(),
		       ((constant *) copied_item)->getType(),
		       copied_item->getId(),
		       ((constant *) copied_item)->getValue());

	break;
      }
    case VARIABLE:
      {
	if(collisionDetected(copied_item, last_pos))
	  status_bar->message("Invalid paste location.", 3000);
	else
	  drawVariable(last_pos.x(),
		       last_pos.y(),
		       ((variable *) copied_item)->getType(),
		       ((variable *) copied_item)->isGlobal(),
		       copied_item->getId(),
		       ((variable *) copied_item)->getName());

	break;
      }
    case BREAK:
      {
	if(collisionDetected(copied_item, last_pos))
	  status_bar->message("Invalid paste location.", 3000);
	else
	  drawBreak(last_pos.x(),
		    last_pos.y(),
		    copied_item->getId());

	break;
      }
    case CONTINUE:
      {
	if(collisionDetected(copied_item, last_pos))
	  status_bar->message("Invalid paste location.", 3000);
	else
	  drawContinue(last_pos.x(),
		       last_pos.y(),
		       copied_item->getId());

	break;
      }
    case RETURN:
      {
	if(collisionDetected(copied_item, last_pos))
	  status_bar->message("Invalid paste location.", 3000);
	else
	  drawReturn(last_pos.x(),
		     last_pos.y(),
		     copied_item->getId());

	break;
      }
    default:
      break;
    }

  canvas()->update();
}

/*
** -- askAboutDelete() --
*/

bool editor::askAboutDelete(void)
{
  /*
  ** Returns true if the user wishes to delete the selected item.
  */

  bool delete_disabled = false;
  settings_window *sw = NULL;

  /*
  ** Is the user sure?
  */

  if((sw = new settings_window()) != NULL)
    {
      sw->readAttribute(options_reader::DELETE_PROMPT_DISABLED, delete_disabled);
      delete sw;
    }
  else
    delete_disabled = false;

  if(!delete_disabled)
    if(QMessageBox::information
       (this, "Question",
	"Are you sure that you wish to delete the selected object?",
	QMessageBox::Yes, QMessageBox::No,
	QMessageBox::Cancel | QMessageBox::Default) != QMessageBox::Yes)
      return false;

  return true;
}
