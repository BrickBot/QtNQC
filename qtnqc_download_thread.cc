/*
** -- C++ Includes --
*/

#include <iostream.h>

/*
** -- Local Includes --
*/

#include <qtnqc_main.h>
#include <qtnqc_download_thread.h>

/*
** -- Local Structures --
*/

struct curl_struct
{
  char *data;
  size_t data_len;
};

static size_t saveVersionToMemory(void *, size_t, size_t, void *);

/*
** -- saveVersionToMemory() --
*/

static size_t saveVersionToMemory(void *ptr, size_t size,
				  size_t nmemb, void *data)
{
  size_t realsize = size * nmemb;
  struct curl_struct *mem = (struct curl_struct *) data;

  mem->data = (char *) malloc(mem->data_len + realsize + 1);

  if(mem->data != NULL)
    {
      snprintf(mem->data, mem->data_len + realsize + 1, "%s", (char *) ptr);
      mem->data_len += realsize + 1;
    }

  return realsize;
}

/*
** -- run() --
*/

void qtnqc_download_thread::run(void)
{
  CURL *curl = NULL;
  CURLcode res = (CURLcode) 0;
  struct curl_struct data;

  version = "";
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  if(curl != NULL)
    {
      data.data = NULL;
      data.data_len = 0;
      curl_easy_setopt(curl, CURLOPT_URL,
		       "http://qtnqc.sourceforge.net/VERSION");
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, saveVersionToMemory);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &data);
      res = curl_easy_perform(curl);

      if(res == 0)
	{
	  version = QString(data.data);
	  version.remove(' ');
	  version.remove('\n');
	}

      free(data.data);
      curl_easy_cleanup(curl);
    }
}

/*
** -- getVersion() --
*/

QString qtnqc_download_thread::getVersion(void)
{
  return version;
}

/*
** -- ~qtnqc_download_thread()
*/

qtnqc_download_thread::~qtnqc_download_thread()
{
}
