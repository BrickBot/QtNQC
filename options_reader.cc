#include <settings.h>
#include <options_reader.h>

QStringList options_reader::keys;

/*
** -- options_reader() --
*/

options_reader::options_reader(void)
{
  contents.setAutoDelete(true);
  initializeKeys();
}

/*
** -- ~options_reader() --
*/

options_reader::~options_reader()
{
  contents.clear();
}

/*
** -- initializeKeys() --
*/

void options_reader::initializeKeys(void)
{
  if(!keys.contains("auto_save"))
    keys.append("auto_save");

  if(!keys.contains("save_geometry"))
    keys.append("save_geometry");

  if(!keys.contains("time_option"))
    keys.append("time_option");

  if(!keys.contains("width"))
    keys.append("width");

  if(!keys.contains("height"))
    keys.append("height");

  if(!keys.contains("x-coord"))
    keys.append("x-coord");

  if(!keys.contains("y-coord"))
    keys.append("y-coord");

  if(!keys.contains("fw-x-coord"))
    keys.append("fw-x-coord");

  if(!keys.contains("fw-y-coord"))
    keys.append("fw-y-coord");

  if(!keys.contains("tw-x-coord"))
    keys.append("tw-x-coord");

  if(!keys.contains("tw-y-coord"))
    keys.append("tw-y-coord");

  if(!keys.contains("nqc_location"))
    keys.append("nqc_location");

  if(!keys.contains("delete_prompt_disabled"))
    keys.append("delete_prompt_disabled");
}

/*
** -- read() --
*/

int options_reader::read(void)
{
  QFile qf;
  QString key = "";
  QString str = "";
  QString *item = NULL;
  QString value = "";
  QTextStream qts;

  qf.setName(settings::getConfigurationFile());

  if(!qf.open(IO_ReadOnly))
    return 1;

  contents.clear();
  qts.setDevice(&qf);

  while(!qts.eof())
    {
      str = qts.readLine().stripWhiteSpace();

      if(str.startsWith("#"))
	continue;

      if(!str.contains("="))
	continue;

      key = str.left(str.find("=")).stripWhiteSpace();
      value = str.right(str.length() - str.find("=") - 1).stripWhiteSpace();

      if((item = new QString(value)) != NULL)
	contents.insert(key, item);
    }

  qf.close();
  return 0;
}

/*
** -- getContents() --
*/

QDict<QString> options_reader::getContents(void)
{
  return contents;
}
