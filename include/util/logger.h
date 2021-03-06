#ifndef LOGGER_H_
#define LOGGER_H_

#include "semaphore.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/sem.h>

#define L_DEBUG 0
#define L_WARNING 1
#define L_ERROR 2

#define LOG_LEVEL 0

void logger_end();
void _log(int logLevel, const char *file, int line, const char *fmt, ...);

#define log_debug(...)  _log(L_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_warning(...) _log(L_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...)  _log(L_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#endif
