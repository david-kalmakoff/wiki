#include "logging.h"

void log_pre_print(int level, const char *file, const char *function,
                   int line) {
  // Display level with colors
  switch (level) {
  case VERBOSE:
    printf("%sVERBOSE%s", KWHT, KNRM);
    break;
  case DEBUG:
    printf("%sDEBUG%s", KBLU, KNRM);
    break;
  case INFO:
    printf("%sINFO %s", KCYN, KNRM);
    break;
  case WARN:
    printf("%sWARN %s", KYEL, KNRM);
    break;
  case ERROR:
    printf("%sERROR%s", KRED, KNRM);
    break;
  case TODO:
    printf("%sTODO %s", KMAG, KNRM);
    break;
  }

  // Display time
  time_t now = time(NULL);
  struct tm *info = localtime(&now);

  printf(" ");
  printf("%d-%02d-%02dT%02d:%02d:%02d", info->tm_year + 1900, info->tm_mon,
         info->tm_mday, info->tm_hour, info->tm_min, info->tm_sec);
  printf(": ");

  printf("%s%s:%d[%s]%s \"", KYEL, file, line, function, KNRM);
}

void log_post_print() { printf("\"\n"); }
