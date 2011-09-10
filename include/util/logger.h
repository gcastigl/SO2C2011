#ifndef LOGGER_H_
#define LOGGER_H_

#include "semaphore.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/sem.h>

#define LOG_GON 1
#define LOG_JP 2
#define LOG_QCHO 3

#define L_DEBUG 0
#define L_ERROR 1
#define VERBOSITY LOG_JP


void logger_end();
void _log(int verboseLevel, int logLevel, const char *file, int line, const char *fmt, ...);

#define log_debug(verboseLevel, ...)  _log(verboseLevel, L_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...)  _log(0, L_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#endif
