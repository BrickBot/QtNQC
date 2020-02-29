#ifndef _QTNQC_MAIN_H_
#define _QTNQC_MAIN_H_

#define VERSION "1.00"

/*
** -- Qt Classes --
*/

class QLayout;
class QGroupBox;
class QPopupMenu;
class QStatusBar;

#include <qcursor.h>
#include <qdialog.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qfiledialog.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qapplication.h>

/*
** -- Local Includes --
*/

#include <editor.h>
#include <diagram_writer.h>
#include <settings_window.h>
#include <qtnqc_download_thread.h>

class qtnqc_main: public QMainWindow
{
  Q_OBJECT

 public:
  qtnqc_main(void);
  ~qtnqc_main();
  bool isSaved(void);
  void init(QApplication *);
  void setSaved(const bool);
  void showMain(void);
  QMainWindow *getMainWindow(void);
  QValueList<QString> &getUsedTaskNames(void);
  QValueList<QString> &getUsedFunctionNames(void);
  QValueList<QString> &getUsedSubroutineNames(void);
  static int current_id;
  static int nextId(void);
  static void quit(const char *, const char *, const int, const int);
  static void prevId(void);
  static void cleanup(void);

 private:
  bool saved;
  editor *edit;
  QString filename;
  QThread *save_thread;
  QStatusBar *statusbar;
  QMainWindow *mainwindow;
  QValueList<QString> used_task_names;
  QValueList<QString> used_function_names;
  QValueList<QString> used_subroutine_names;
  qtnqc_download_thread *dwnld_thread;
  void newFile(void);
  void writeToFile(void);
  void readFromFile(void);
  virtual void closeEvent(QCloseEvent *);

 public slots:
  void quit(void);

 private slots:
  bool slotNew(void);
  void find(void);
  void undo(void);
  void enlarge(void);
  void funcPal(void);
  void showCode(void);
  void toolsPal(void);
  void slotOpen(void);
  void slotSave(void);
  void showAbout(void);
  void getUpdates(void);
  void slotSaveAs(void);
  void showSettings(void);
};

#endif
