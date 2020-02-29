#include <list_objects.h>

/*
** -- list_objects() --
*/

list_objects::list_objects(QWidget *parent, editor *edit_arg): QDialog(parent)
{
  QGroupBox *group_box = NULL;
  QGridLayout *layout1 = NULL;
  QHBoxLayout *layout2 = NULL;

  edit = edit_arg;
  setCaption("Canvas Items");

  if((layout1 = new QGridLayout(this, 2, 1, 0, -1, "layout1")) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((layout2 = new QHBoxLayout(64)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((goto_loc = new QPushButton("Goto", this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((cancel = new QPushButton("Close", this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((remove = new QPushButton("Delete", this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  connect(cancel, SIGNAL(clicked()), this, SLOT(close_cb(void)));
  connect(remove, SIGNAL(clicked()), this, SLOT(delete_cb(void)));
  connect(goto_loc, SIGNAL(clicked()), this, SLOT(goto_cb(void)));
  cancel->setDefault(true);

  if((group_box = new QGroupBox(1, QGroupBox::Horizontal,
				"", this, "group_box")) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((table = new QTable(0, 3, group_box)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  /*
  ** Configure the table's properties.
  */

  table->setReadOnly(true);
  table->setSelectionMode(QTable::SingleRow);
  table->verticalHeader()->setResizeEnabled(false);
  table->horizontalHeader()->setResizeEnabled(false);
  table->horizontalHeader()->setLabel(0, QString("Item Type"),
				      250);
  table->horizontalHeader()->setLabel(1, QString("Name / Value"),
				      150);
  table->horizontalHeader()->setLabel(2, QString("Coordinates (x, y)"),
				      200);

  /*
  ** Set the layout.
  */

  layout2->addWidget(goto_loc);
  layout2->addWidget(remove);
  layout2->addWidget(cancel);
  layout1->addWidget(group_box, 0, 0);
  layout1->addLayout(layout2, 1, 0);

  /*
  ** Prevent resizing.
  */

  resize(sizeHint().width() - 5, 500);
  setFixedSize(size());
}

/*
** -- ~list_objects() --
*/

list_objects::~list_objects()
{
}

/*
** -- close_cb() --
*/

void list_objects::close_cb(void)
{
  hide();
}

/*
** -- goto_cb() --
*/

void list_objects::goto_cb(void)
{
  int x = -1;
  int y = -1;

  getXY(&x, &y);

  if(x == -1 || y == -1)
    return;

  edit->scrollTo(x, y);
}

/*
** -- getXY() --
*/

void list_objects::getXY(int *x, int *y)
{
  int x1 = 0;
  int y1 = 0;
  QString str = "";

  if(table->currentRow() == -1)
    return;
  else
    str = table->text(table->currentRow(), 2);

  str.remove('(');
  str.remove(')');
  x1 = atoi(str.section(',', 0));
  y1 = atoi(str.section(',', 1));

  if(x != NULL)
    *x = x1;

  if(y != NULL)
    *y = y1;
}

/*
** -- delete_cb() --
*/

void list_objects::delete_cb(void)
{
  int x = -1;
  int y = -1;
  bool delete_disabled = false;
  settings_window *sw = NULL;

  getXY(&x, &y);

  if(x == -1 || y == -1)
    return;

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
      return;

  edit->deleteSelection(x, y);
}

/*
** -- fill() --
*/

void list_objects::fill(void)
{
  int ct = 0;
  char buffer[128];
  unsigned int i = 0;

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  table->setNumRows(edit->getGates()->count() +
		    edit->getLoops()->count() +
		    edit->getConstants()->count() +
		    edit->getVariables()->count() +
		    edit->getTasks()->count() +
		    edit->getSubroutines()->count() +
		    edit->getFunctions()->count() +
		    edit->getBreaks()->count() +
		    edit->getContinues()->count() +
		    edit->getReturns()->count());

  for(i = 0; i < (unsigned int) edit->getGates()->count(); i++, ct++)
    {
      (void) snprintf(buffer, sizeof(buffer), "%s",
		      edit->getGates()->at(i)->getTypeName());
      table->setText(ct, 0, buffer);
      table->setText(ct, 1, "");
      (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
		      edit->getGates()->at(i)->getX(),
		      edit->getGates()->at(i)->getY());
      table->setText(ct, 2, buffer);
    }

  for(i = 0; i < (unsigned int) edit->getLoops()->count(); i++, ct++)
    {
      (void) snprintf(buffer, sizeof(buffer), "%s",
		      edit->getLoops()->at(i)->getTypeName());
      table->setText(ct, 0, buffer);
      table->setText(ct, 1, "");
      (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
		      edit->getLoops()->at(i)->getX(),
		      edit->getLoops()->at(i)->getY());
      table->setText(ct, 2, buffer);
    }

  for(i = 0; i < (unsigned int) edit->getConstants()->count(); i++, ct++)
    {
      (void) snprintf(buffer, sizeof(buffer), "%s",
		      edit->getConstants()->at(i)->getTypeName());
      table->setText(ct, 0, buffer);
      (void) snprintf(buffer, sizeof(buffer), "%d",
		      edit->getConstants()->at(i)->getValue());
      table->setText(ct, 1, buffer);
      (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
		      edit->getConstants()->at(i)->getX(),
		      edit->getConstants()->at(i)->getY());
      table->setText(ct, 2, buffer);
    }

  for(i = 0; i < (unsigned int) edit->getVariables()->count(); i++, ct++)
    {
      (void) snprintf(buffer, sizeof(buffer), "%s",
		      edit->getVariables()->at(i)->getTypeName());
      table->setText(ct, 0, buffer);
      (void) snprintf(buffer, sizeof(buffer), "%s",
		      edit->getVariables()->at(i)->getName().data());
      table->setText(ct, 1, buffer);
      (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
		      edit->getVariables()->at(i)->getX(),
		      edit->getVariables()->at(i)->getY());
      table->setText(ct, 2, buffer);
    }

  for(i = 0; i < (unsigned int) edit->getTasks()->count(); i++, ct++)
    {
      (void) snprintf(buffer, sizeof(buffer), "%s",
		      edit->getTasks()->at(i)->getTypeName());
      table->setText(ct, 0, buffer);
      (void) snprintf(buffer, sizeof(buffer), "%s",
		      edit->getTasks()->at(i)->getName().data());
      table->setText(ct, 1, buffer);
      (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
		      edit->getTasks()->at(i)->getX(),
		      edit->getTasks()->at(i)->getY());
      table->setText(ct, 2, buffer);
    }

  for(i = 0; i < (unsigned int) edit->getSubroutines()->count(); i++, ct++)
    {
      (void) snprintf(buffer, sizeof(buffer), "%s",
		      edit->getSubroutines()->at(i)->getTypeName());
      table->setText(ct, 0, buffer);
      (void) snprintf(buffer, sizeof(buffer), "%s",
		      edit->getSubroutines()->at(i)->getName().data());
      table->setText(ct, 1, buffer);
      (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
		      edit->getSubroutines()->at(i)->getX(),
		      edit->getSubroutines()->at(i)->getY());
      table->setText(ct, 2, buffer);
    }

  for(i = 0; i < (unsigned int) edit->getFunctions()->count(); i++, ct++)
    {
      (void) snprintf(buffer, sizeof(buffer), "%s",
		      edit->getFunctions()->at(i)->getTypeName());
      table->setText(ct, 0, buffer);
      (void) snprintf(buffer, sizeof(buffer), "%s",
		      edit->getFunctions()->at(i)->getName().data());
      table->setText(ct, 1, buffer);
      (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
		      edit->getFunctions()->at(i)->getX(),
		      edit->getFunctions()->at(i)->getY());
      table->setText(ct, 2, buffer);
    }

  for(i = 0; i < (unsigned int) edit->getBreaks()->count(); i++, ct++)
    {
      (void) snprintf(buffer, sizeof(buffer), "%s",
		      "Break Statement");
      table->setText(ct, 0, buffer);
      table->setText(ct, 1, "");
      (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
		      edit->getBreaks()->at(i)->getX(),
		      edit->getBreaks()->at(i)->getY());
      table->setText(ct, 2, buffer);
    }

  for(i = 0; i < (unsigned int) edit->getContinues()->count(); i++, ct++)
    {
      (void) snprintf(buffer, sizeof(buffer), "%s",
		      "Continue Statement");
      table->setText(ct, 0, buffer);
      table->setText(ct, 1, "");
      (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
		      edit->getContinues()->at(i)->getX(),
		      edit->getContinues()->at(i)->getY());
      table->setText(ct, 2, buffer);
    }

  for(i = 0; i < (unsigned int) edit->getReturns()->count(); i++, ct++)
    {
      (void) snprintf(buffer, sizeof(buffer), "%s",
		      "Return Statement");
      table->setText(ct, 0, buffer);
      table->setText(ct, 1, "");
      (void) snprintf(buffer, sizeof(buffer), "(%d, %d)",
		      edit->getReturns()->at(i)->getX(),
		      edit->getReturns()->at(i)->getY());
      table->setText(ct, 2, buffer);
    }

  QApplication::restoreOverrideCursor();
}

/*
** -- clear() --
*/

void list_objects::clear(void)
{
  while(table->numRows() > 0)
    table->removeRow(0);
}
