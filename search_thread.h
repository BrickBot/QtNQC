#ifndef _SEARCH_THREAD_H_
#define _SEARCH_THREAD_H_

#include <qthread.h>

class search_thread: public QThread
{
 public:
  search_thread(void);
  ~search_thread();
  void set_search_how(const int);
  virtual void run();

 private:
  int search_how;
};

#endif
