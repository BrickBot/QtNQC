#ifndef _OPTIONS_READER_H_
#define _OPTIONS_READER_H_

/*
** -- Qt Includes --
*/

#include <qdict.h>
#include <qfile.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtextstream.h>

class options_reader
{
 public:
  enum keynames {AUTO_SAVE = 0,
		 SAVE_GEOMETRY,
		 TIME_OPTION,
		 WIDTH,
		 HEIGHT,
		 X_COORD,
		 Y_COORD,
		 FW_X_COORD,
		 FW_Y_COORD,
		 TW_X_COORD,
		 TW_Y_COORD,
		 NQC_LOCATION,
		 DELETE_PROMPT_DISABLED};
  static QStringList keys;

 public:
  options_reader(void);
  ~options_reader();
  int read(void);
  QDict<QString> getContents(void);

 private:
  static void initializeKeys(void);
  QString filename;
  QDict<QString> contents;
};

#endif
