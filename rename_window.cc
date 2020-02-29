/*
** -- C++ Includes --
*/

#include <iostream.h>

/*
** -- Local Includes --
*/

#include <rename_window.h>

/*
** -- rename_window() --
*/

extern qtnqc_main *qmain;

rename_window::rename_window(QWidget *parent): QDialog(parent)
{
  QLabel *name_label = NULL;
  QGroupBox *group_box1 = NULL;
  QGroupBox *group_box2 = NULL;
  QGridLayout *layout1 = NULL;
  QHBoxLayout *layout2 = NULL;

  obj = NULL;
  name_str = "";

  if((layout1 = new QGridLayout(this, 2, 1, 0, -1, "layout1")) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((layout2 = new QHBoxLayout(64)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((group_box1 = new QGroupBox(1, QGroupBox::Horizontal,
				 "", this, "group_box1")) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((group_box2 = new QGroupBox(1, QGroupBox::Vertical,
				 "", this, "group_box2")) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((ok = new QPushButton("Apply", group_box2)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((help = new QPushButton("Help", group_box2)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((cancel = new QPushButton("Close", group_box2)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  connect(cancel, SIGNAL(clicked()), this, SLOT(close_cb(void)));
  connect(help, SIGNAL(clicked()), this, SLOT(help_cb(void)));
  connect(ok, SIGNAL(clicked()), this, SLOT(ok_cb(void)));
  cancel->setDefault(true);

  if((name_label = new QLabel("Name", group_box1)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((name = new QLineEdit(group_box1)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  /*
  ** Set the maximum number of characters allowed.
  */

  name->setMaxLength(16);

  /*
  ** Set the layout.
  */

  layout2->addWidget(group_box2);
  layout1->addWidget(group_box1, 0, 0);
  layout1->addLayout(layout2, 1, 0);

  /*
  ** Prevent resizing.
  */

  resize(sizeHint());
  setFixedSize(size());
}

/*
** -- ok_cb() --
*/

void rename_window::ok_cb(void)
{
  QRegExp exp("^[a-zA-Z_]+[0-9a-zA-Z_]*$");

  if(name->text() == name_str)
    {
      /*
      ** Same name.
      */

      close_cb();
      return;
    }

  /*
  ** Is the name already taken or is it invalid?
  ** Prepend either an "f_" or "t_" to the name
  ** when generating NQC code for functions or tasks.
  ** As for variables, prepend a "v_" when generating
  ** the NQC code.
  ** For subroutines, prepend a "s_" when generating
  ** the NQC code.
  */

  if(exp.search(name->text()) == -1)
    (void) QMessageBox::critical(this, "Error",
				 "Invalid name.",
				 QMessageBox::Ok | QMessageBox::Default,
				 0);
  else if((qmain->getUsedFunctionNames().contains(name->text()) ||
	   qmain->getUsedSubroutineNames().contains(name->text()) ||
	   qmain->getUsedTaskNames().contains(name->text())) &&
	  (obj != NULL && (obj->getType() == FUNCTION ||
			   obj->getType() == SUBROUTINE ||
			   obj->getType() == TASK)))
    (void) QMessageBox::critical(this, "Error",
				 "Name already in use.",
				 QMessageBox::Ok | QMessageBox::Default,
				 0);
  else
    {
      qmain->setSaved(false);
      name_str = name->text();
      close_cb();

      if(obj != NULL)
	obj->update(name_str);
      else
	cerr << "Variable obj is NULL. File " << __FILE__
	     << ", line " << __LINE__ << "." << endl;
    }
}

/*
** -- help() --
*/

void rename_window::help_cb(void)
{
}

/*
** -- getNameStr() --
*/

QString rename_window::getNameStr(void)
{
  return name_str;
}

/*
** -- setNameStr() --
*/

void rename_window::setNameStr(const QString &name_str_arg)
{
  name_str = name_str_arg;
  name->setText(name_str);
}

/*
** -- setObject() --
*/

void rename_window::setObject(const object *obj_arg)
{
  obj = (object *) obj_arg;
}

/*
** -- ~rename_window() --
*/

rename_window::~rename_window()
{
  delete name;
  delete ok;
  delete help;
  delete cancel;
}

/*
** -- close_cb() --
*/

void rename_window::close_cb(void)
{
  hide();
}
