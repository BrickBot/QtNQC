/*
** Copyright (c) 2004, 2005, 2006 Alexis Megas
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
** -- C Includes --
*/

extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
}

/*
** -- C++ Includes --
*/

#include <iostream.h>

/*
** -- Local Includes --
*/

#include <types.h>
#include <object.h>
#include <qtnqc_main.h>
#include <tools_window.h>
#include <options_reader.h>
#include <function_window.h>

/*
** -- Local Variables --
*/

int qtnqc_main::current_id = 1;
qtnqc_main *qmain = NULL;
QApplication *qapp = NULL;
tools_window *tw = NULL;
options_reader *optread = NULL;
function_window *fw = NULL;

/*
** -- main() --
*/

int main(int argc, char *argv[])
{
  /*
  ** Create the user interface.
  */

  QApplication::setColorSpec(QApplication::CustomColor);

  if((qapp = new QApplication(argc, argv)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((qmain = new qtnqc_main()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  qmain->init(qapp);
  QObject::connect(qapp, SIGNAL(lastWindowClosed()), qapp, SLOT(quit(void)));

  /*
  ** Enter an endless loop.
  */

  qapp->exec();

  /*
  ** Return.
  */

  return EXIT_SUCCESS;
}

/*
** -- qtnqc_main() --
*/

qtnqc_main::qtnqc_main(void)
{
  saved = true;
  edit = NULL;
  save_thread = NULL;
  options_reader::keys.clear();
}

/*
** -- closeEvent() --
*/

void qtnqc_main::closeEvent(QCloseEvent *e)
{
  if(e != NULL)
    ;

  quit();
}

/*
** -- init() --
*/

void qtnqc_main::init(QApplication *qapp)
{
  QCanvas *canvas = NULL;
  QCanvas *fcanvas = NULL;
  QCanvas *tcanvas = NULL;
  QPopupMenu *editmenu = NULL;
  QPopupMenu *filemenu = NULL;
  QPopupMenu *helpmenu = NULL;
  QPopupMenu *viewmenu = NULL;
  QPopupMenu *optionmenu = NULL;
  QMainWindow *dialog1 = NULL;
  QMainWindow *dialog2 = NULL;
  settings_window *sw = NULL;

  qapp->connect(qapp, SIGNAL(lastWindowClosed()), qapp,
			SLOT(quit(void)));
  dwnld_thread = NULL;

  this->setUpdatesEnabled(true);
  statusbar = this->statusBar();

  if(statusbar == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((dialog1 = new QMainWindow
      (this, "function_window",
       Qt::WType_Dialog | Qt::WStyle_Dialog)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if(dialog1->statusBar() != NULL)
    dialog1->statusBar()->message("None");
  else
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  dialog1->setCaption("Functions Palette");

  if((dialog2 = new QMainWindow
      (this, "tools_window",
       Qt::WType_Dialog | Qt::WStyle_Dialog)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if(dialog2->statusBar() != NULL)
    dialog2->statusBar()->message("Cursor");
  else
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  dialog2->setCaption("Tools Palette");

  if((fcanvas = new QCanvas(210, 510)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((tcanvas = new QCanvas(210, 60)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((fw = new function_window(fcanvas, dialog1)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((sw = new settings_window(this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((tw = new tools_window(tcanvas, dialog2)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  /*
  ** Set the application's main widget.
  */

  qapp->setMainWidget(this);

  if((canvas = new QCanvas(editor::MAX_WIDTH, editor::MAX_HEIGHT)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((edit = new editor(true, canvas, -1, this,
			this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  /*
  ** edit is the main editor.
  */

  edit->setMain(true);

  /*
  ** Set the title of the main window.
  */

  this->setCaption("QtNQC " + QString(VERSION));

  /*
  ** Create menus.
  */

  if((editmenu = new QPopupMenu(this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((filemenu = new QPopupMenu(this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);


  if((helpmenu = new QPopupMenu(this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((viewmenu = new QPopupMenu(this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  if((optionmenu = new QPopupMenu(this)) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);

  filemenu->insertItem(QPixmap("./icons/open.png"),
		       "&Open Diagram",
		       this,
		       SLOT(slotOpen(void)),
		       Qt::CTRL + Qt::Key_O);
  filemenu->insertItem(QPixmap("./icons/new.png"),
		       "&New Diagram", this, SLOT(slotNew(void)),
		       Qt::CTRL + Qt::Key_N);
  filemenu->insertSeparator();
  filemenu->insertItem(QPixmap("./icons/save.png"),
		       "&Save", this, SLOT(slotSave(void)),
		       Qt::CTRL + Qt::Key_S);
  filemenu->insertItem(QPixmap("./icons/saveas.png"),
		       "Save &As", this, SLOT(slotSaveAs(void)),
		       Qt::CTRL + Qt::Key_A);
  filemenu->insertSeparator();
  filemenu->insertItem(QPixmap("./icons/exit.png"),
		       "&Quit QtNQC", this, SLOT(quit(void)),
		       Qt::CTRL + Qt::Key_Q);
  viewmenu->insertItem(QPixmap("./icons/functions.png"),
		       "&Functions Palette", this,
		       SLOT(funcPal(void)));
  viewmenu->insertItem(QPixmap("./icons/tools.png"),
		       "&Tools Palette", this,
		       SLOT(toolsPal(void)));
  viewmenu->insertSeparator();
  viewmenu->insertItem(QPixmap("./icons/generate.png"),
		       "&NQC Code", this, SLOT(showCode(void)));
  editmenu->insertItem(QPixmap("./icons/enlarge.png"),
		       "&Enlarge Canvas", this, SLOT(enlarge(void)),
		       Qt::CTRL + Qt::Key_E);
  editmenu->insertItem(QPixmap("./icons/find.png"),
		       "&Find", this, SLOT(find(void)),
		       Qt::CTRL + Qt::Key_F);
  editmenu->insertItem(QPixmap("./icons/undo.png"),
		       "&Undo", this, SLOT(undo(void)),
		       Qt::CTRL + Qt::Key_U);
  helpmenu->insertItem(QPixmap("./icons/download.png"),
		       "&Version Check", this, SLOT(getUpdates(void)));
  helpmenu->insertItem(QPixmap("./icons/help.png"),
		       "&About", this, SLOT(showAbout(void)));
  optionmenu->insertItem(QPixmap("./icons/settings.png"),
			 "&Program Settings", this,
			 SLOT(showSettings(void)),
			 Qt::CTRL + Qt::Key_P);

  /*
  ** Add the menus.
  */

  this->menuBar()->insertItem("&File", filemenu);
  this->menuBar()->insertItem("&View", viewmenu);
  this->menuBar()->insertItem("&Edit", editmenu);
  this->menuBar()->insertItem("&Options", optionmenu);
  this->menuBar()->insertSeparator();
  this->menuBar()->insertItem("&Help", helpmenu);

  /*
  ** Last-minute initializations.
  */

  filename = "";

  /*
  ** Show the main window.
  */

  this->setCentralWidget(edit);

  /*
  ** Read the configuration file.
  */

  if((optread = new options_reader()) == NULL)
    qtnqc_main::quit("Memory allocation failure", __FILE__, __LINE__,
		     EXIT_FAILURE);
  else if(optread->read() != 0)
    {
      this->show();
      this->resize(this->sizeHint());
      (void) QMessageBox::warning(this, "Warning",
				  "An error occurred while processing the "
				  "configuration file (.qtnqc.conf).",
				  QMessageBox::Ok | QMessageBox::Default,
				  0);
    }
  else
    {
      /*
      ** Update the settings window. This will also
      ** update the user interface and start any background processes.
      */

      sw->update(true);

      if(!this->isVisible())
	this->show();
    }

  this->setMinimumSize(editor::MAX_WIDTH / 4, editor::MAX_HEIGHT / 4);
  dialog1->setCentralWidget(fw);
  dialog1->resize(dialog1->sizeHint());
  dialog1->setFixedSize(dialog1->size());
  dialog1->show();
  dialog2->setCentralWidget(tw);
  dialog2->resize(dialog2->sizeHint());
  dialog2->setFixedSize(dialog2->size());
  dialog2->show();
  delete sw;
}

/*
** -- cleanup() --
*/

void qtnqc_main::cleanup(void)
{
  delete qmain;

  if(qapp != NULL)
    qapp->unlock(true);

  delete qapp;
  current_id = 1;
  qmain = NULL;
  qapp = NULL;
}

/*
** -- quit() --
*/

void qtnqc_main::quit(void)
{
  bool exit_program = true;

  if(!saved)
    if(QMessageBox::information
       (this, "Question",
	"You have not saved your changes.\nAre you sure you wish to exit?",
	QMessageBox::Yes, QMessageBox::No,
	QMessageBox::Cancel | QMessageBox::Default) == QMessageBox::Yes)
      exit_program = true;
    else
      exit_program = false;

  if(exit_program)
    {
      cleanup();
      cout << "QtNQC has exited successfully." << endl;
      exit(EXIT_SUCCESS);
    }
}

/*
** -- quit() --
*/

void qtnqc_main::quit(const char *msg, const char *file, const int line,
		      const int code)
{
  cleanup();

  if(msg != NULL && strlen(msg) > 0)
    cerr << msg << " in file " << file << ", line " << line << "." << endl;
  else
    cerr << "An unknown error occurred in file " << file << ", line "
	 << line << "." << endl;

  exit(code);
}

/*
** -- ~qtnqc_main() --
*/

qtnqc_main::~qtnqc_main()
{
  used_task_names.clear();
  used_function_names.clear();
  used_subroutine_names.clear();
  delete optread;
  delete fw;
  delete tw;
  delete edit;
  delete dwnld_thread;
  delete save_thread;
  dwnld_thread = NULL;
  save_thread = NULL;
  edit = NULL;
  fw = NULL;
  tw = NULL;
  optread = NULL;
}

/*
** -- funcPal() --
*/

void qtnqc_main::funcPal(void)
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
** -- getMainWindow() --
*/

QMainWindow *qtnqc_main::getMainWindow(void)
{
  return this;
}

/*
** -- showCode() --
*/

void qtnqc_main::showCode(void)
{
}

/*
** -- toolsPal() --
*/

void qtnqc_main::toolsPal(void)
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
** -- getUpdates() --
*/

void qtnqc_main::getUpdates(void)
{
  QString msg = "";
  QString version = "";

  if((dwnld_thread = new qtnqc_download_thread()) != NULL)
    {
      getMainWindow()->statusBar()->message("Downloading version information...");
      qapp->setOverrideCursor(QCursor(Qt::WaitCursor));
      dwnld_thread->start();

      while(dwnld_thread->running())
	(void) usleep(100000);

      version = dwnld_thread->getVersion();

      if(!version.isEmpty())
	msg = "The latest version of QtNQC is " + version + ".\nYou have version " +
	  QString(VERSION) + ".";
      else
	msg = "Unable to retrieve version information.";

      delete dwnld_thread;
      dwnld_thread = NULL;
      qapp->restoreOverrideCursor();
      getMainWindow()->statusBar()->clear();
      (void) QMessageBox::information(this, "QtNQC Version", msg,
				      QMessageBox::Ok | QMessageBox::Default);
    }
}

/*
** -- showAbout() --
*/

void qtnqc_main::showAbout(void)
{
  char buffer[256];

  (void) snprintf(buffer, sizeof(buffer),
		  "QtNQC Version %s.\n"
		  "Copyright (c) 2004, 2005, 2006 Alexis Megas.\n"
		  "Designed for NQC 3.1 r5.\n"
		  "Icons created by GNOME.",
		  VERSION);
  (void) QMessageBox::information(this, "About QtNQC", buffer,
				  QMessageBox::Ok | QMessageBox::Default);
}

/*
** -- setSaved() --
*/

void qtnqc_main::setSaved(const bool saved_arg)
{
  QString tmp = "";

  saved = saved_arg;

  if(!filename.isEmpty())
    tmp = "QtNQC " + QString(VERSION) + ": " + filename;
  else
    tmp = "QtNQC " + QString(VERSION);

  if(saved)
    tmp = tmp.remove(" *");
  else
    tmp += " *";

  this->setCaption(tmp);
}

/*
** -- isSaved() --
*/

bool qtnqc_main::isSaved(void)
{
  return saved;
}

/*
** -- enlarge() --
*/

void qtnqc_main::enlarge(void)
{
  edit->enlarge();
}

/*
** -- showMain() --
*/

void qtnqc_main::showMain(void)
{
  this->raise();
  fw->getParent()->raise();
  tw->getParent()->raise();
}

void qtnqc_main::newFile(void)
{
  filename = "";
  current_id = 1;
  this->setCaption("QtNQC " + QString(VERSION));
  qapp->setOverrideCursor(QCursor(Qt::WaitCursor));
  edit->cleanup();
  used_task_names.clear();
  used_function_names.clear();
  used_subroutine_names.clear();
  saved = true;
  qapp->restoreOverrideCursor();
}

/*
** -- slotNew() --
*/

bool qtnqc_main::slotNew(void)
{
  /*
  ** Clear everything. Prompt the user if a save is necessary.
  */

  bool new_canvas = true;

  if(!saved)
    if(QMessageBox::information
       (this, "Question",
	"You have not saved your changes.\nAre you sure you wish to "
	"continue?",
	QMessageBox::Yes, QMessageBox::No,
	QMessageBox::Cancel | QMessageBox::Default) == QMessageBox::Yes)
      new_canvas = true;
    else
      new_canvas = false;

  if(new_canvas)
    newFile();

  return new_canvas;
}

/*
** -- undo() --
*/

void qtnqc_main::undo(void)
{
}

/*
** -- find() --
*/

void qtnqc_main::find(void)
{
  /*
  ** Run this in a thread.
  */
}

/*
** -- slotOpen() --
*/

void qtnqc_main::slotOpen(void)
{
  bool new_canvas = true;
  QString str = "";

  if(!saved)
    if(QMessageBox::information
       (this, "Question",
	"You have not saved your changes.\nAre you sure you wish to "
	"continue?",
	QMessageBox::Yes, QMessageBox::No,
	QMessageBox::Cancel | QMessageBox::Default) == QMessageBox::Yes)
      new_canvas = true;
    else
      new_canvas = false;

  if(new_canvas)
    if(!(str = QFileDialog::getOpenFileName("", "*.qtnqc",
					    this)).isEmpty())
      {
	newFile();
	filename = str;
	readFromFile();
      }
}

/*
** -- slotSave() --
*/

void qtnqc_main::slotSave(void)
{
  if(filename.isEmpty())
    slotSaveAs();
  else
    writeToFile();
}

/*
** -- slotSaveAs() --
*/

void qtnqc_main::slotSaveAs(void)
{
  QString str = "";

  if(!(str = QFileDialog::getSaveFileName(filename,
					  "*.qtnqc", this)).isEmpty())
    {
      filename = str;

      if(!filename.endsWith(".qtnqc"))
	filename += ".qtnqc";

      writeToFile();
    }
}

/*
** -- showSettings() --
*/

void qtnqc_main::showSettings(void)
{
  settings_window *sw = NULL;

  if((sw = new settings_window(this)) != NULL)
    {
      sw->update(false);
      sw->exec();
      delete sw;
    }
  else
    (void) QMessageBox::warning(this, "Warning",
				"Unable to create a settings_window object.",
				QMessageBox::Ok | QMessageBox::Default,
				0);
}

/*
** -- writeToFile() --
*/

void qtnqc_main::writeToFile(void)
{
  diagram_writer *diagw = NULL;

  qapp->setOverrideCursor(QCursor(Qt::WaitCursor));

  if((diagw = new diagram_writer(filename, edit, this)) != NULL)
    saved = diagw->isSaved();

  delete diagw;
  qapp->restoreOverrideCursor();
}

/*
** -- readFromFile() --
*/

void qtnqc_main::readFromFile(void)
{
  QFile qf;
  QString error = "";

  current_id = 1;
  qf.setName(filename);

  if(!qf.open(IO_ReadOnly | IO_Translate))
    {
      error = "Unable to open " + filename + ".";
      goto error_label;
    }

  qapp->setOverrideCursor(QCursor(Qt::WaitCursor));
  edit->readFile(qf);
  qf.close();
  qapp->restoreOverrideCursor();
  this->setCaption("QtNQC " + QString(VERSION) + ": " + filename);

 error_label:

  if(!error.isEmpty())
    {
      (void) QMessageBox::critical(this, "Error", error,
				   QMessageBox::Ok | QMessageBox::Default,
				   0);
    }
  else
    saved = true;
}

/*
** -- nextId() --
*/

int qtnqc_main::nextId(void)
{
  return current_id++;
}

/*
** -- prevId() --
*/

void qtnqc_main::prevId(void)
{
  if(current_id > 1)
    current_id -= 1;
}

/*
** -- getUsedTaskNames() --
*/

QValueList<QString> &qtnqc_main::getUsedTaskNames(void)
{
  return used_task_names;
}

/*
** -- getUsedFunctionNames(void) --
*/

QValueList<QString> &qtnqc_main::getUsedFunctionNames(void)
{
  return used_function_names;
}

/*
** -- getUsedSubroutineNames() --
*/

QValueList<QString> &qtnqc_main::getUsedSubroutineNames(void)
{
  return used_subroutine_names;
}
