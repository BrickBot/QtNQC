#ifndef _QTNQC_DOWNLOAD_THREAD_H_
#define _QTNQC_DOWNLOAD_THREAD_H_

/*
** -- Qt Classes --
*/

#include <qthread.h>

/*
** -- libcurl Includes --
*/

#include <curl/curl.h>

class qtnqc_download_thread: public QThread
{
 public:
  ~qtnqc_download_thread();
  QString getVersion(void);
  virtual void run(void);

 private:
  QString version;
};

#endif
