#include "util/logger.h"

static FILE* logFile = NULL;
static int initialized = 0;
int semKey = 0x20605202;
int semId;

void initLogging() {
    if (initialized) {
        return;
    }
    
    #ifdef LOG_TO_FILE
        logFile = fopen("log.txt", "a");
    #else
        logFile = stderr;
    #endif
    
    semId = semaphore_create(semKey, 1, 0600);
    initialized = 1;
}