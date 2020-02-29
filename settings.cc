#include <settings.h>

QString settings::filename = "";

/*
** -- getConfigurationFile() --
*/

QString settings::getConfigurationFile(void)
{
  struct passwd *pwd = NULL;

  if(filename.isEmpty())
    if((pwd = getpwuid(getuid())) != NULL)
      {
	filename = pwd->pw_dir;
	filename += "/.qtnqc.conf";
      }

  return filename;
}
