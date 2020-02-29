#ifndef _FUNCTION_WINDOW_H_
#define _FUNCTION_WINDOW_H_

#define ITEMS_COLS 4

class QCanvas;
template<class> class QValueList;

class function_window: public QCanvasView
{
 public:
  function_window(QCanvas *, QWidget * = 0, const char * = 0, WFlags = 0);
  ~function_window();
  int getType(void);
  void hideDialog(void);
  void displayDialog(void);
  QWidget *getParent(void);

 protected:
  void contentsMouseMoveEvent(QMouseEvent *);
  void contentsMousePressEvent(QMouseEvent *);

 private:
  int type;
  QCanvas *canvas;
  QWidget *parent;
  QValueList<int> items;
  QValueList<const char *> descriptions;
  QCanvasRectangle *selection;
  static const int square_s = 50;
};

#endif
