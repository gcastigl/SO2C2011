#include "util/logger.h"

static FILE* logFile = NULL;
static int initialized = 0;
char *loggerSem = "logger";
static sem_t* sem;
static char *logLevelString[] = { "DEBUG", "WARNING", "ERROR" };

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
        fflush(logFile);

        sem_post(sem);
    }
}
