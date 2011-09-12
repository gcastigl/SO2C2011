#include "util/logger.h"
#include <execinfo.h>

static FILE* logFile = NULL;
static int initialized = 0;
char *loggerSem = "logger";
static sem_t* sem;
static char *logLevelString[] = { "DEBUG", "WARNING", "ERROR" };
void print_trace(int logLevel);

void logger_init() {
    if (initialized) {
        return;
    }
    #ifdef LOG_TO_FILE
        unlink("log.txt");
        logFile = fopen("log.txt", "a");
    #else
        logFile = stderr;
    #endif
    
    sem = semaphore_create(loggerSem);
    sem_post(sem);
    initialized = 1;
}

void logger_end() {
    #ifdef LOG_TO_FILE
        fclose(logFile);
    #endif
    
    if (sem) {
        sem_close(sem);
    }
}

void _log(int logLevel, const char *file, int line, const char *fmt, ...) {
    logger_init();
    va_list ap;
    va_start(ap, fmt);
    
    if (logLevel >= LOG_LEVEL) {
        sem_wait(sem);
        fprintf(logFile, "[%s] %s:%d: ", logLevelString[logLevel], file, line);
        vfprintf(logFile, fmt, ap);
        fprintf(logFile, "\n");
        print_trace(logLevel);
        fflush(logFile);
        sem_post(sem);
    }
}

void print_trace(int logLevel) {
	#ifndef DEBUG_MODE
		return;
	#endif
	if (logLevel < L_ERROR){
		return;
	}
	void *array[10];
	size_t size;
	char **strings;
	size_t i;
	size = backtrace(array, 10);
	strings = backtrace_symbols(array, size);
	fprintf(logFile, "Stack: [errno %d: %s]", errno, strerror(errno));
	for (i = 0; i < size; i++)
		fprintf(logFile, "\t%s\n", strings[i]);
	free(strings);
}
