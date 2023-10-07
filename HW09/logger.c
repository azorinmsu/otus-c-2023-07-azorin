#include "logger.h"
#include <time.h>
#include <stdarg.h>
#include <execinfo.h>

#define BT_BUF_SIZE 100

FILE* consoleOutput = NULL;
FILE* fileOutput = NULL;

static char getLevelChar(LogLevel level) {
  switch(level) {
    case LogLevel_TRACE: return 'T';
    case LogLevel_DEBUG: return 'D';
    case LogLevel_INFO: return 'I';
    case LogLevel_WARN: return 'W';
    case LogLevel_ERROR: return 'E';
    case LogLevel_FATAL: return 'F';
    default: return ' ';
  }
}

void init_consoleLogger(FILE* output) {
  output = (output != NULL) ? output : stdout;
  consoleOutput = output;
}

void init_fileLogger(char* filename) {
  if (filename == NULL) {
    return;
  }

  if (fileOutput != NULL) {
    fclose(fileOutput); 
  }

  fileOutput = fopen(filename, "a");
  if (fileOutput == NULL) {
    fprintf(stderr, "ERROR: logger: Failed to open file %s\n", filename);
    return;
  }
}

static void vflogStacktrace(FILE* output) {
   int nptrs;
   void* buffer[BT_BUF_SIZE];
   char** strings;

   nptrs = backtrace(buffer, BT_BUF_SIZE);
   strings = backtrace_symbols(buffer, nptrs);
  
   for (int i = 0; i < nptrs; ++i) {
    fprintf(output, "%s\n", strings[i]);
   }
}

static void vflog(FILE* output, char level, char* filename, int line, char* currentTime, char* msg) {
  fprintf(output, "%c %s %s: %d:", level, currentTime, filename, line);

  va_list arg;
  vfprintf(output, msg, arg);
 
  if (level == 'E') {
    vflogStacktrace(output);
  }

  fprintf(output, "\n");
}

void logger_log(LogLevel level, char* filename, int line, char* msg) {
  time_t rawtime;
  struct tm * timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  char* currentTime = asctime(timeinfo);

  char levelc = getLevelChar(level);
  
  if (consoleOutput != NULL) {
    vflog(consoleOutput, levelc, filename, line, currentTime, msg);
  }

  if (fileOutput != NULL) {
    vflog(fileOutput, levelc, filename, line, currentTime, msg);
  }
}

