#ifndef LOGGER_H_
#define LOGGER_H_

#include "semaphore.h"
#include <stdio.h>
#include <stdarg.h>

#define L_DEBUG 0
#define L_ERROR 1
void endLogging();
void _log(int logLevel, const char *file, int line, const char *fmt, ...);

#define log_debug(...)  _log(L_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...)  _log(L_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#endif
