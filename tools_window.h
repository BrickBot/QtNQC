#ifndef _TOOLS_WINDOW_H_
#define _TOOLS_WINDOW_H_

#define ITEMS_COLS 4

class QCanvas;
template<class> class QValueList;

class tools_window: public QCanvasView
{
 public:
  tools_window(QCanvas *, QWidget * = 0, const char * = 0, WFlags = 0);
  ~tools_window();
  int getType(void);
  void hideDialog(void);
  void displayDialog(void);
  QWidget *getParent(void);

 protected:
  void contentsMousePressEvent(QMouseEvent *);

 private:
  int type;
  QCanvas *canvas;
  QWidget *parent;
  QValueList<int> items;
  QCanvasRectangle *selection;
};

#endif
