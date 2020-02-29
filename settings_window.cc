#include <settings.h>
#include <qtnqc_main.h>
#include <options_reader.h>
#include <function_window.h>
#include <settings_window.h>

extern QApplication *qapp;
extern tools_window *tw;
extern options_reader *optread;
extern function_window *fw;

/*
** -- settings_window() --
*/

settings_window::settings_window(QMainWindow *parent):
  QTabDialog(parent, "settings_window", Qt::WType_Dialog)
{
  /*
  ** Create a tabbed window for saving various settings.
  */

  auto_save = NULL;
  save_geometry = NULL;
  time_options = NULL;
  nqc_location = NULL;
  disable_delete_prompt = NULL;
  setCaption("QtNQC Settings");
  setOkButton("Apply");
  setCancelButton("Cancel");
  connect(this, SIGNAL(applyButtonPressed()), this, SLOT(saveOptions()));
  connect(this, SIGNAL(cancelButtonPressed()), this, SLOT(cancelOptions()));

  /*
  ** Add the various tabs.
  */

  setTab1();
  setTab2();
  setTab3();
  resize(sizeHint());
  setFixedSize(size());
}

/*
** -- cancelOptions() --
*/

void settings_window::cancelOptions(void)
{
}

/*
** -- settings_window() --
*/

settings_window::settings_window(void)
{
  optread->read();
  auto_save = NULL;
  save_geometry = NULL;
  time_options = NULL;
  nqc_location = NULL;
  disable_delete_prompt = NULL;
}

/*
** -- ~settings_window() --
*/

settings_window::~settings_window(void)
{
  delete auto_save;
  delete save_geometry;
  delete time_options;
  delete nqc_location;
  delete disable_delete_prompt;
}

/*
** -- setTab1() --
*/

void settings_window::setTab1(void)
{
  int i = 0;
  char buffer[8];
  QLabel *label1 = NULL;
  QFrame *tab = NULL;
  QGridLayout *layout = NULL;

  if((tab = new QFrame(this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((layout = new QGridLayout(tab, 2, 5)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((auto_save = new QCheckBox(tab)) == NULL)
     qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  connect(auto_save, SIGNAL(clicked()), this, SLOT(auto_save_cb()));

  if((time_options = new QComboBox(true, tab)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  time_options->setEditable(false);

  if((label1 = new QLabel(tab)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((save_geometry = new QCheckBox(tab)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  label1->setText(" Minute(s)");
  layout->setMargin(5);
  layout->setColSpacing(1, 5);
  layout->addWidget(auto_save, 0, 0);
  layout->addWidget(time_options, 0, 2);
  layout->addWidget(label1, 0, 3);
  layout->addWidget(save_geometry, 1, 0);

  for(i = 1; i <= 60; i++)
    {
      (void) snprintf(buffer, sizeof(buffer), "%d", i);
      time_options->insertItem(buffer);
    }

  time_options->setEnabled(false);
  auto_save->setText("Enable Automatic Backup Every ");
  save_geometry->setText("Save Geometry");
  addTab(tab, "General");
}


/*
** -- setTab2() --
*/

void settings_window::setTab2(void)
{
  QFrame *tab = NULL;
  QLabel *label1 = NULL;
  QGridLayout *layout = NULL;
  QPushButton *button = NULL;

  if((tab = new QFrame(this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((layout = new QGridLayout(tab, 2, 3)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);
  
  if((label1 = new QLabel(tab)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((nqc_location = new QLineEdit(tab)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((button = new QPushButton(tab)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  connect(button, SIGNAL(clicked()), this, SLOT(selectNQCBin()));
  label1->setText("Program Location");
  button->setPixmap(QPixmap("./icons/open.png"));
  nqc_location->setReadOnly(true);
  layout->setMargin(5);
  layout->setColSpacing(1, 5);
  layout->addWidget(label1, 0, 0);
  layout->addWidget(nqc_location, 1, 0);
  layout->addWidget(button, 1, 2);
  addTab(tab, "NQC Executable");
}

/*
** -- setTab3() --
*/

void settings_window::setTab3(void)
{
  QFrame *tab = NULL;
  QGridLayout *layout = NULL;

  if((tab = new QFrame(this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((layout = new QGridLayout(tab, 1, 1)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((disable_delete_prompt = new QCheckBox(tab)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  disable_delete_prompt->setText("Disable Delete Confirmation Prompt");
  layout->setMargin(5);
  layout->addWidget(disable_delete_prompt, 0, 0);
  addTab(tab, "Editing");
}

/*
** -- saveOptions() --
*/

void settings_window::saveOptions(void)
{
  QFile qf;
  QString error = "";
  QString filename = "";
  QTextStream qts;

  /*
  ** Write the data to the specified file.
  */

  if((filename = settings::getConfigurationFile()).isEmpty())
    {
      error = "Unable to retrieve the name of your home directory.";
      goto error_label;
    }

  qf.setName(filename);

  if(!qf.open(IO_Truncate | IO_WriteOnly))
    {
      error = "Unable to open " + filename + ".";
      goto error_label;
    }

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  qts.setDevice(&qf);
  qts << "# Edit at your own risk." << endl;

  if(save_geometry->isChecked())
    {
      qts << "save_geometry = true" << endl;
      qts << "height = " << ((QMainWindow *) parent())->height() << endl;
      qts << "width = " << ((QMainWindow *) parent())->width() << endl;
      qts << "x-coord = " << ((QMainWindow *) parent())->x() << endl;
      qts << "y-coord = " << ((QMainWindow *) parent())->y() << endl;
      qts << "fw-x-coord = " << fw->getParent()->x() << endl;
      qts << "fw-y-coord = " << fw->getParent()->y() << endl;
      qts << "tw-x-coord = " << tw->getParent()->x() << endl;
      qts << "tw-y-coord = " << tw->getParent()->y() << endl;
    }
  else
    qts << "save_geometry = false" << endl;

  if(auto_save->isChecked())
    {
      qts << "auto_save = true" << endl;
      qts << "time_option = "
	  << time_options->text(time_options->currentItem())
	  << endl;
    }
  else
    qts << "auto_save = false" << endl;

  qts << "nqc_location = " << nqc_location->text() << endl;

  if(disable_delete_prompt->isChecked())
    qts << "delete_prompt_disabled = true" << endl;
  else
    qts << "delete_prompt_disabled = false" << endl;

  qf.close();
  optread->read();
  QApplication::restoreOverrideCursor();

 error_label:

  if(!error.isEmpty())
    (void) QMessageBox::critical((QWidget *) parent(), "Error", error,
				 QMessageBox::Ok | QMessageBox::Default,
				 0);
  else
    {
      hide();
      (void) QMessageBox::information((QWidget *) parent(), "Information",
				      "The settings has been saved.",
				      QMessageBox::Ok | QMessageBox::Default,
				      0);
    }
}

/*
** -- auto_save_cb() --
*/

void settings_window::auto_save_cb(void)
{
  time_options->setEnabled(auto_save->isChecked());
}

/*
** -- update() --
*/

void settings_window::update(const bool update_parent)
{
  int i = 0;
  int x = -1;
  int y = -1;
  int num = 0;
  int width = -1;
  int height = -1;
  bool ok = false;
  QPoint pt1;
  QPoint pt2;
  QPoint moveto;
  QString str = "";

  pt1.setX(-1);
  pt1.setY(-1);
  pt2.setX(-1);
  pt2.setY(-1);

  for(i = 0; i < (int) options_reader::keys.size(); i++)
    {
      if(!optread->getContents()[options_reader::keys[i]])
	continue;

      str = *(optread->getContents()[options_reader::keys[i]]);

      switch(i)
	{
	case options_reader::AUTO_SAVE:
	  {
	    if(str == "true")
	      auto_save->setChecked(true);
	    else
	      auto_save->setChecked(false);

	    time_options->setEnabled(auto_save->isChecked());
	    break;
	  }
	case options_reader::SAVE_GEOMETRY:
	  {
	    if(str == "true")
	      save_geometry->setChecked(true);
	    else
	      save_geometry->setChecked(false);

	    break;
	  }
	case options_reader::TIME_OPTION:
	  {
	    num = str.toInt(&ok);

	    if(ok && auto_save->isChecked())
	      {
		num -= 1;

		if(num >= 0 && num < (int) time_options->count())
		  time_options->setCurrentItem(num);
		else
		  time_options->setCurrentItem(0);
	      }
	    else
	      time_options->setCurrentItem(0);

	    break;
	  }
	case options_reader::HEIGHT:
	  {
	    num = str.toInt(&ok);

	    if(ok && save_geometry->isChecked() && num >= editor::MAX_HEIGHT / 4)
	      height = num;

	    break;
	  }
	case options_reader::WIDTH:
	  {
	    num = str.toInt(&ok);

	    if(ok && save_geometry->isChecked() && num >= editor::MAX_WIDTH / 4)
	      width = num;

	    break;
	  }
	case options_reader::X_COORD:
	  {
	    num = str.toInt(&ok);

	    if(ok && save_geometry->isChecked() && num >= 0)
	      x = num;

	    break;
	  }
	case options_reader::Y_COORD:
	  {
	    num = str.toInt(&ok);

	    if(ok && save_geometry->isChecked() && num >= 0)
	      y = num;

	    break;
	  }
	case options_reader::FW_X_COORD:
	  {
	    num = str.toInt(&ok);

	    if(ok && save_geometry->isChecked() && num >= 0)
	      pt1.setX(num);

	    break;
	  }
	case options_reader::FW_Y_COORD:
	  {
	    num = str.toInt(&ok);

	    if(ok && save_geometry->isChecked() && num >= 0)
	      pt1.setY(num);

	    break;
	  }
	case options_reader::TW_X_COORD:
	  {
	    num = str.toInt(&ok);

	    if(ok && save_geometry->isChecked() && num >= 0)
	      pt2.setX(num);

	    break;
	  }
	case options_reader::TW_Y_COORD:
	  {
	    num = str.toInt(&ok);

	    if(ok && save_geometry->isChecked() && num >= 0)
	      pt2.setY(num);

	    break;
	  }
	case options_reader::NQC_LOCATION:
	  {
	    nqc_location->setText(str);
	    break;
	  }
	case options_reader::DELETE_PROMPT_DISABLED:
	  {
	    if(str == "true")
	      disable_delete_prompt->setChecked(true);
	    else
	      disable_delete_prompt->setChecked(false);

	    break;
	  }
	default:
	  break;
	}
    }

  if(update_parent)
    {
      if(x > -1 && y > -1 && height > -1 && width > -1)
	{
	  ((QMainWindow *) parent())->resize(width, height);
	  moveto.setX(x);
	  moveto.setY(y);
	  ((QMainWindow *) parent())->move(moveto);
	}

      if(pt1.x() > -1 && pt1.y() > -1)
	fw->getParent()->move(pt1);

      if(pt2.x() > -1 && pt2.y() > -1)
	tw->getParent()->move(pt2);
    }
}

/*
** -- selectNQCBin() --
*/

void settings_window::selectNQCBin(void)
{
  QString str = "";

  if(!(str = QFileDialog::getOpenFileName("/usr", "*", this)).isEmpty())
    nqc_location->setText(str);
}
