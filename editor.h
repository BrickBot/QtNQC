#ifndef _EDITOR_H_
#define _EDITOR_H_

/*
** -- Qt Classes --
*/

class QPopupMenu;
class list_objects;
class rename_window;
template<class> class QPtrList;
template<class> class QValueList;

/*
** -- Qt Includes --
*/

#include <qtimer.h>
#include <qcanvas.h>
#include <qcursor.h>

/*
** -- Local Includes --
*/

#include <gate.h>
#include <loop.h>
#include <task.h>
#include <wire.h>
#include <break.h>
#include <return.h>
#include <constant.h>
#include <continue.h>
#include <function.h>
#include <variable.h>
#include <dragobject.h>
#include <file_class.h>
#include <subroutine.h>
#include <list_objects.h>
#include <qtnqc_limits.h>
#include <tools_window.h>
#include <rename_window.h>

class editor: public QCanvasView
{
  Q_OBJECT

 public:
  static const int MOVE_UP = 1;
  static const int MOVE_DOWN = 2;
  static const int MOVE_LEFT = 3;
  static const int MOVE_RIGHT = 4;
  static const int MAX_WIDTH = 2048;
  static const int MAX_HEIGHT = 2048;

  editor(const bool, QCanvas *, const int, QWidget *,
	 QWidget *, const char * = 0,
	 WFlags = 0);
  ~editor();
  bool isMainEditor(void);
  bool collisionDetected(object *, const QPoint &);
  void clear(void);
  void cleanup(void);
  void enlarge(void);
  void setMain(const bool);
  void drawGate(const int, const int, const int, const int = -1);
  void drawLoop(const int, const int, const int, QPtrList<file_class> *,
		const int = -1, const QString & = "",
		const int = 0, const int = 0);
  void drawTask(const int, const int, const int, QPtrList<file_class> *,
		const int = -1, const QString & = "",
		const int = 0, const int = 0);
  void drawSubroutine(const int, const int, const int, QPtrList<file_class> *,
		      const int = -1, const QString & = "",
		      const int = 0, const int = 0);
  void readFile(QFile &);
  void scrollTo(const int, const int);
  void drawBreak(const int, const int, const int = -1);
  void drawGeneric(const file_class *, QPtrList<file_class> *);
  void writeToFile(QTextStream &, QString &);
  void drawReturn(const int, const int, const int = -1);
  void drawConstant(const int, const int, const int, const int = -1,
		    const int = 0);
  void drawContinue(const int, const int, const int = -1);
  void drawFunction(const int, const int, const int,
		    const int = 0, const int = 0);
  void drawVariable(const int, const int, const int, const bool, const int,
		    const QString &);
  void updateLister(void);
  void createChildren(QPtrList<file_class> *);
  void deleteSelection(const int, const int);
  void updateFuncSubmenu(void);
  void updateTaskSubmenu(void);
  void updateSubroutineSubmenu(void);
  void updateVariableNamesList(const QString &);

  QPtrList<gate> *getGates(void)
    {
      return gates;
    };

  QPtrList<break_obj> *getBreaks(void)
    {
      return breaks;
    };

  QPtrList<continue_obj> *getContinues(void)
    {
      return continues;
    };

  QPtrList<return_obj> *getReturns(void)
    {
      return returns;
    };

  QPtrList<loop> *getLoops(void)
    {
      return loops;
    };

  QPtrList<task> *getTasks(void)
    {
      return tasks;
    };

  QPtrList<subroutine> *getSubroutines(void)
    {
      return subroutines;
    };

  QPtrList<constant> *getConstants(void)
    {
      return constants;
    };

  QPtrList<function> *getFunctions(void)
    {
      return functions;
    };

  QPtrList<variable> *getVariables(void)
    {
      return variables;
    };

 private:
  int parent_id;
  int copied_item_type;
  int selected_item_type;
  bool is_main_editor;
  object *copied_item;
  object *selected_item;
  QPoint last_pos;
  QPoint moving_start;
  QTimer *sel_timer;
  QWidget *dialog_parent;
  QPtrList<gate> *gates;
  QPtrList<loop> *loops;
  QPtrList<task> *tasks;
  QPtrList<wire> *wires;
  QPtrList<subroutine> *subroutines;
  QPtrList<constant> *constants;
  QPtrList<function> *functions;
  QPtrList<variable> *variables;
  QPtrList<break_obj> *breaks;
  QPtrList<file_class> fc_list;
  QPtrList<return_obj> *returns;
  QPtrList<continue_obj> *continues;
  dragobject *drag;
  QLineEdit *condition;
  QPopupMenu *editmenu;
  QPopupMenu *func_submenu;
  QPopupMenu *task_submenu;
  QPopupMenu *subroutine_submenu;
  QValueList<QString> used_variable_names;
  list_objects *lister;
  rename_window *rw;
  QCanvasRectangle *selection;
  bool askAboutDelete(void);

 protected:
  void dropEvent(QDropEvent *);
  void dragMoveEvent(QDragMoveEvent *);
  void keyPressEvent(QKeyEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void dragEnterEvent(QDragEnterEvent *);
  void mousePressEvent(QMouseEvent *);
  void contentsMousePressEvent(QMouseEvent *);

 signals:
  void status(const QString &);

 private slots:
  void slotEdit(void);
  void setGlobal(void);
  void slotDelete(void);
  void slotRename(void);
  void slotAddTask(void);
  void slotAddSubroutine(void);
  void slotListAll(void);
  void slotShowHelp(void);
  void slotShowMain(void);
  void slotShowFuncs(void);
  void slotShowTools(void);
  void slotCopyObject(void);
  void slotAddFunction(void);
  void slotPasteObject(void);
  void slotSelTimerTimeout(void);
};

#endif
