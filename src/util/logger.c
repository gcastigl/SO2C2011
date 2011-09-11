#include "util/logger.h"

static FILE* logFile = NULL;
static int initialized = 0;
int semKey = 0x20605202;
int semId;
static char *logLevelString[] = { "DEBUG", "ERROR" };

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
    
    semId = semaphore_create(semKey, 1, 0666);
    semctl(semId, 0, SETVAL, 1);
    initialized = 1;
}

void logger_end() {
    #ifdef LOG_TO_FILE
        fclose(logFile);
    #endif
    
    if (semId) {
        semaphore_destroy(semId);
    }
}

void _log(int logLevel, const char *file, int line, const char *fmt, ...) {
    logger_init();
    va_list ap;
    va_start(ap, fmt);
    
    if (logLevel >= LOG_LEVEL) {
        semaphore_decrement(semKey, 0);
        fprintf(logFile, "[%s] %s:%d: ", logLevelString[logLevel], file, line);
        vfprintf(logFile, fmt, ap);
        fprintf(logFile, "\n");
        fflush(logFile);

        semaphore_increment(semKey, 0);
    }
}
