#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
  #define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
  #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define LOG_TRACE(msg) logger_log(LogLevel_TRACE, __FILENAME__, __LINE__, msg)
#define LOG_DEBUG(msg) logger_log(LogLevel_DEBUG, __FILENAME__, __LINE__, msg)
#define LOG_INFO(msg) logger_log(LogLevel_INFO, __FILENAME__, __LINE__, msg)
#define LOG_WARN(msg) logger_log(LogLevel_WARN, __FILENAME__, __LINE__, msg)
#define LOG_ERROR(msg) logger_log(LogLevel_ERROR, __FILENAME__, __LINE__, msg)
#define LOG_FATAL(msg) logger_log(LogLevel_FATAL, __FILENAME__, __LINE__, msg)

typedef enum {
  LogLevel_TRACE,
  LogLevel_DEBUG,
  LogLevel_INFO,
  LogLevel_WARN,
  LogLevel_ERROR,
  LogLevel_FATAL
} LogLevel;

void init_consoleLogger(FILE* output);

void init_fileLogger(char* filename);

void logger_log(LogLevel level, char* filename, int line, char* msg);

#endif
