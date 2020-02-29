#ifndef _SETTINGS_WINDOW_H_
#define _SETTINGS_WINDOW_H_

/*
** -- Qt Includes --
*/

#include <qlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qtabdialog.h>
#include <qmainwindow.h>
#include <qmessagebox.h>

/*
** -- Local Includes --
*/

#include <options_reader.h>

extern options_reader *optread;

class settings_window: public QTabDialog
{
  Q_OBJECT

 public:
  settings_window(void);
  settings_window(QMainWindow *);
  ~settings_window();
  bool isDeleteDisabled(void);
  void update(const bool);

  /*
  ** -- readAttribute() --
  */

  template<class T>
  void readAttribute(options_reader::keynames attr, T &value)
  {
    QString str = "";

    if(!optread->getContents()[options_reader::keys[attr]])
      switch(attr)
	{
	case options_reader::DELETE_PROMPT_DISABLED:
	  {
	    value = false;
	    return;
	  }
	default:
	  return;
	}

    str = *(optread->getContents()[options_reader::keys[attr]]);

    switch(attr)
      {
      case options_reader::DELETE_PROMPT_DISABLED:
	{
	  if(str == "true")
	    value = true;
	  else
	    value = false;

	  break;
	}
      default:
	break;
      }
  }

 private:
  bool delete_prompt_disabled;
  QCheckBox *auto_save;
  QCheckBox *save_geometry;
  QCheckBox *disable_delete_prompt;
  QComboBox *time_options;
  QLineEdit *nqc_location;
  void setTab1(void);
  void setTab2(void);
  void setTab3(void);

 private slots:
  void saveOptions(void);
  void auto_save_cb(void);
  void selectNQCBin(void);
  void cancelOptions(void);
};

#endif
