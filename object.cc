/*
** -- C/C++ Includes --
*/

extern "C"
{
#include <stdlib.h>
}

/*
** -- Qt Includes --
*/

#include <qlabel.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qvalidator.h>
#include <qmainwindow.h>

/*
** -- Local Includes --
*/

#include <editor.h>
#include <object.h>
#include <qtnqc_main.h>

extern qtnqc_main *qmain;

/*
** -- object() --
*/

object::object(const int type_arg, const int x0, const int y0,
	       QCanvas *canvas_arg, const char *type_name_arg,
	       QPtrList<file_class> *list, const int obj_id_arg,
	       const int cwidth_arg, const int cheight_arg)
{
  int cwidth = editor::MAX_WIDTH;
  int cheight = editor::MAX_HEIGHT;
  char buffer[256];
  QLabel *name_label = NULL;
  QGroupBox *group_box1 = NULL;
  QGroupBox *group_box2 = NULL;
  QPopupMenu *filemenu = NULL;
  QPopupMenu *editmenu = NULL;
  QValidator *validator = NULL;
  QGridLayout *layout1 = NULL;
  QHBoxLayout *layout2 = NULL;

  memset(type_name, '\0', sizeof(type_name) / sizeof(type_name[0]));

  if(type_name_arg != NULL)
    (void) snprintf(type_name, sizeof(type_name), "%s", type_name_arg);

  x = x0;
  y = y0;
  type = type_arg;
  written = false;
  dialog = NULL;
  edit = NULL;
  view = NULL;
  edit_canvas = NULL;
  parent_canvas = canvas_arg;
  item_name = "";
  dots = NULL;
  textfield = NULL;
  button = NULL;
  cancel_button = NULL;
  cvalue = 0;
  condition = NULL;

  if(cwidth_arg != 0)
    cwidth = cwidth_arg;

  if(cheight_arg != 0)
    cheight = cheight_arg;

  if(obj_id_arg == -1)
    obj_id = qtnqc_main::nextId();
  else
    obj_id = obj_id_arg;

  if(type != WIRE)
    {
      if((dots = new QPtrList<QCanvasEllipse>()) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      dots->setAutoDelete(false);
    }

  if(!(type == CONSTANT || type == VARIABLE ||
       type == FUNCTION || type == TASK || type == SUBROUTINE ||
       type == FOR_LOOP || type == WHILE_LOOP ||
       type == DO_WHILE_LOOP || type == REPEAT_LOOP))
    return;

  (void) memset(buffer, '\0', sizeof(buffer) / sizeof(buffer[0]));

  if(type == CONSTANT)
    {
      if((dialog = new QDialog()) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);
    }
  else if((dialog = new QMainWindow()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if(type == CONSTANT)
    {
      if((layout1 = new QGridLayout(dialog, 2, 1, 0, -1, "layout1")) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      if((layout2 = new QHBoxLayout(64)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      if((validator = new QIntValidator(SHRT_MIN, SHRT_MAX,
					this)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      if((group_box1 = new QGroupBox(2, QGroupBox::Vertical,
				     "", dialog, "group_box1")) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      if((name_label = new QLabel("Value (16-bit Integer)",
				  group_box1)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      if((textfield = new QLineEdit(group_box1)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      if((group_box2 = new QGroupBox(1, QGroupBox::Vertical,
				     "", dialog, "group_box2")) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      if((button = new QPushButton("OK", group_box2)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      if((cancel_button = new QPushButton("Cancel", group_box2)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      textfield->setValidator(validator);
      connect(button, SIGNAL(clicked()), this, SLOT(ok_cb(void)));
      connect(cancel_button, SIGNAL(clicked()), this, SLOT(cancel_cb(void)));
      textfield->setMaxLength(6);
      (void) snprintf(buffer, sizeof(buffer), "Constant (%d, %d)",
		      getX(), getY());
      layout2->addWidget(group_box2);
      layout1->addWidget(group_box1, 0, 0);
      layout1->addLayout(layout2, 1, 0);
      dialog->setCaption(buffer);
      dialog->resize(dialog->sizeHint().width() + 100,
		     dialog->sizeHint().height());
      dialog->setFixedSize(dialog->size());
    }
  else
    {
      dialog->setMinimumSize(editor::MAX_WIDTH / 4, editor::MAX_HEIGHT / 4);

      if(type == FUNCTION)
	(void) snprintf(buffer, sizeof(buffer), "Function (%s)",
			item_name.data());
      else if(type == TASK)
	(void) snprintf(buffer, sizeof(buffer), "Task (%s)",
			item_name.data());
      else if(type == SUBROUTINE)
	(void) snprintf(buffer, sizeof(buffer), "Subroutine (%s)",
			item_name.data());
      else if(type == FOR_LOOP)
	(void) snprintf(buffer, sizeof(buffer), "For-Loop (%d, %d)",
			getX(), getY());
      else if(type == WHILE_LOOP)
	(void) snprintf(buffer, sizeof(buffer), "While-Loop (%d, %d)",
			getX(), getY());
      else if(type == DO_WHILE_LOOP)
	(void) snprintf(buffer, sizeof(buffer), "Do-While-Loop (%d, %d)",
			getX(), getY());
      else if(type == REPEAT_LOOP)
	(void) snprintf(buffer, sizeof(buffer), "Repeat-Loop (%d, %d)",
			getX(), getY());	    

      if(strlen(buffer) > 0)
	dialog->setCaption(buffer);
      else
	dialog->setCaption("QtNQC " + QString(VERSION));

      if((edit_canvas = new QCanvas(cwidth, cheight)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      if((view = new QCanvasView(edit_canvas)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      if((filemenu = new QPopupMenu(dialog)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      if((editmenu = new QPopupMenu(dialog)) == NULL)
	qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			 EXIT_FAILURE);

      filemenu->insertItem(QPixmap("./icons/exit.png"),
			   "Close &Window", this, SLOT(ok_cb(void)),
			   Qt::CTRL + Qt::Key_W);
      editmenu->insertItem(QPixmap("./icons/enlarge.png"),
			   "&Enlarge Canvas", this, SLOT(enlarge(void)),
			   Qt::CTRL + Qt::Key_E);
      ((QMainWindow *) dialog)->menuBar()->insertItem("&File", filemenu);
      ((QMainWindow *) dialog)->menuBar()->insertItem("&Edit", editmenu);

      if(type == FOR_LOOP || type == REPEAT_LOOP ||
	 type == DO_WHILE_LOOP || type == WHILE_LOOP)
	{
	  if((group_box1 = new QGroupBox(2, QGroupBox::Vertical,
					 "", dialog, "group_box1")) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  if((group_box2 = new QGroupBox(2, QGroupBox::Horizontal,
					 "", group_box1,
					 "group_box2")) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  if((name_label = new QLabel("Condition Statement",
				      group_box2)) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  if((condition = new QLineEdit(group_box2)) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  condition->setMaxLength(1024);
	  connect(condition, SIGNAL(textChanged(const QString &)),
		  this, SLOT(setCondition(const QString &)));


	  if((edit = new editor(false, edit_canvas, obj_id,
				group_box1, dialog)) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  ((QMainWindow *) dialog)->setCentralWidget(group_box1);
	}
      else
	{
	  if((edit = new editor(false, edit_canvas, obj_id, dialog,
				dialog)) == NULL)
	    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
			     EXIT_FAILURE);

	  ((QMainWindow *) dialog)->setCentralWidget(edit);
	}

      if(list != NULL)
	edit->createChildren(list);

      (void) ((QMainWindow *) dialog)->statusBar();
      dialog->resize(editor::MAX_WIDTH / 4, editor::MAX_HEIGHT / 4);
    }
}

/*
** -- ~object() --
*/

object::~object()
{
  qtnqc_main::prevId();
  delete textfield;
  textfield = NULL;
  delete condition;
  condition = NULL;
  delete button;
  button = NULL;
  delete cancel_button;
  cancel_button = NULL;
  delete edit;
  edit = NULL;
  delete view;
  view = NULL;
  delete dialog;
  dialog = NULL;

  if(dots != NULL)
    dots->clear();

  delete dots;
  dots = NULL;
}

/*
** -- getType() --
*/

int object::getType(void)
{
  return type;
}

/*
** -- isWritten() --
*/

bool object::isWritten(void)
{
  return written;
}

/*
** -- setWritten() --
*/

void object::setWritten(const bool written_arg)
{
  written = written_arg;
}

/*
** -- show() --
*/

void object::show(void)
{
  if(dialog != NULL)
    {
      /*
      ** Place the dialog in the foreground.
      */

      updateLabel();
      dialog->hide();
      dialog->show();
    }
}

/*
** -- getX() --
*/

int object::getX(void)
{
  return x;
}

/*
** -- getY() --
*/

int object::getY(void)
{
  return y;
}

/*
** -- updateLabel() --
*/

void object::updateLabel(void)
{
  char buffer[256];

  (void) memset(buffer, '\0', sizeof(buffer) / sizeof(buffer[0]));

  if(type == FUNCTION)
    (void) snprintf(buffer, sizeof(buffer), "Function (%s)",
		    item_name.data());
  else if(type == TASK)
    (void) snprintf(buffer, sizeof(buffer), "Task (%s)",
		    item_name.data());
  else if(type == SUBROUTINE)
    (void) snprintf(buffer, sizeof(buffer), "Subroutine (%s)",
		    item_name.data());
  else if(type == FOR_LOOP)
    (void) snprintf(buffer, sizeof(buffer), "For-Loop (%d, %d)",
		    getX(), getY());
  else if(type == WHILE_LOOP)
    (void) snprintf(buffer, sizeof(buffer), "While-Loop (%d, %d)",
		    getX(), getY());
  else if(type == DO_WHILE_LOOP)
    (void) snprintf(buffer, sizeof(buffer), "Do-While-Loop (%d, %d)",
		    getX(), getY());
  else if(type == REPEAT_LOOP)
    (void) snprintf(buffer, sizeof(buffer), "Repeat-Loop (%d, %d)",
		    getX(), getY());
  else if(type == CONSTANT)
    (void) snprintf(buffer, sizeof(buffer), "Constant (%d, %d)",
		    getX(), getY());

  if(strlen(buffer) > 0 && dialog != NULL)
    dialog->setCaption(buffer);
}

/*
** -- getName() --
*/

QString object::getName(void)
{
  return item_name;
}

/*
** -- update() --
*/

void object::update(const QString &item_name_arg)
{
  QString tmp = "";

  tmp = item_name;
  item_name = item_name_arg;

  if(getType() == FUNCTION)
    {
      /*
      ** Remove the old name and instert the new one.
      */

      qmain->getUsedFunctionNames().remove(tmp);
      qmain->getUsedFunctionNames().append(item_name);
      ((function *) this)->update(item_name);
    }
  else if(getType() == TASK)
    {
      /*
      ** Remove the old name and instert the new one.
      */

      qmain->getUsedTaskNames().remove(tmp);
      qmain->getUsedTaskNames().append(item_name);
      ((task *) this)->update(item_name);
    }
  else if(getType() == SUBROUTINE)
    {
      /*
      ** Remove the old name and instert the new one.
      */

      qmain->getUsedSubroutineNames().remove(tmp);
      qmain->getUsedSubroutineNames().append(item_name);
      ((subroutine *) this)->update(item_name);
    }
  else if(getType() == VARIABLE)
    {
      ((variable *) this)->update(item_name);
      edit->updateVariableNamesList(item_name);
    }

  if(edit != NULL)
    edit->updateLister();
}

/*
** -- getDots() --
*/

QPtrList<QCanvasEllipse> *object::getDots(void)
{
  return dots;
}

/*
** -- setValue() --
*/

void object::setValue(const int value)
{
  char buffer[128];

  if(textfield != NULL)
    {
      (void) snprintf(buffer, sizeof(buffer), "%d", value);
      textfield->setText(buffer);
    }
}

/*
** -- cancel_cb() --
*/

void object::cancel_cb(void)
{
  dialog->hide();
}

/*
** -- ok_cb() --
*/

void object::ok_cb(void)
{
  if(dialog != NULL)
    {
      if(type == CONSTANT)
	{
	  /*
	  ** Update the Constant's value.
	  */

	  cvalue = atoi(textfield->text().data());

	  /*
	  ** Call the update method.
	  */

	  (((constant *) this)->*constUpdatePtr)();
	}

      dialog->hide();
    }
}

/*
** -- setUpdateFunc() --
*/

void object::setConstUpdateFunc(void (constant::*ptr)(void))
{
  constUpdatePtr = ptr;
}

/*
** -- enlarge() --
*/

void object::enlarge(void)
{
  edit->enlarge();
}

/*
** -- pos() --
*/

QPoint object::pos(void)
{
  return QPoint(x, y);
}

/*
** -- setParent() --
*/

void object::setParent(QWidget *parent, const QPoint p)
{
  dialog->reparent(parent, Qt::WType_Dialog, p, false);
}

/*
** -- getTypeName() --
*/

char *object::getTypeName(void)
{
  return type_name;
}

/*
** -- getId() --
*/

int object::getId(void)
{
  return obj_id;
}

/*
** -- writeToFile() --
*/

void object::writeToFile(QTextStream &qts, QString &error,
			 const int parent_id)
{
  qts << "<object>" << endl;
  qts << "<type>" << type << endl;
  qts << "<id>" << obj_id << endl;
  qts << "<x-coord>" << x << endl;
  qts << "<y-coord>" << y << endl;

  if(type == TASK || type == FUNCTION || type == SUBROUTINE)
    {
      qts << "<name>" << item_name << endl;
      qts << "<canvas-width>" << edit_canvas->width() << endl;
      qts << "<canvas-height>" << edit_canvas->height() << endl;
    }

  if(type == CONSTANT)
    qts << "<value>" << cvalue << endl;

  if(type == VARIABLE)
    {
      qts << "<name>" << item_name << endl;
      qts << "<is-global>" << ((variable *) this)->isGlobal() << endl;
    }

  if(type == WHILE_LOOP || type == FOR_LOOP ||
     type == DO_WHILE_LOOP || type == REPEAT_LOOP)
    {
      qts << "<condition_str>" << condition->text() << endl;
      qts << "<canvas-width>" << edit_canvas->width() << endl;
      qts << "<canvas-height>" << edit_canvas->height() << endl;
    }

  qts << "<parent-id>" << parent_id << endl;
  qts << "</object>" << endl;

  if(edit != NULL)
    edit->writeToFile(qts, error);
}

/*
** -- getEditor() --
*/

editor *object::getEditor(void)
{
  return edit;
}

/*
** -- setCondition() --
*/

void object::setCondition(const QString &str)
{
  ((loop *) this)->setCondition(str);
}

/*
** -- getParentCanvas() --
*/

QCanvas *object::getParentCanvas(void)
{
  return parent_canvas;
}
