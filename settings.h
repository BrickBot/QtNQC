#ifndef _SETTINGS_H_
#define _SETTINGS_H_

extern "C"
{
#include <pwd.h>
#include <unistd.h>
};

/*
** -- Qt Includes --
*/

#include <qstring.h>

class settings
{
 public:
  static QString getConfigurationFile(void);

 private:
  static QString filename;
};

#endif
