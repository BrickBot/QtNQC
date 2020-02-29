#ifndef _DIAGRAM_WRITER_H_
#define _DIAGRAM_WRITER_H_

/*
** -- Qt Classes --
*/

class QString;
class QWidget;

/*
** -- Local Classes --
*/

class editor;

/*
** -- Qt Includes --
*/

#include <qfile.h>
#include <qcursor.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qapplication.h>

/*
** -- Local Includes --
*/

#include <editor.h>

class diagram_writer
{
 public:
  diagram_writer(const QString &, editor *, QWidget *);
  bool isSaved(void);

 private:
  bool saved;
};

#endif
