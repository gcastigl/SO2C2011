#include "util/signalHandling.h"
// FIXME:
static int processCount;

void signal_setProcessCount(int count) {
	processCount = count;
}

void signal_handler(int sigVal) {
    switch (sigVal) {
        case SIGSEGV:
            log_error("Segmentation fault");
            signal_abortSimulation(sigVal);
            kill(getppid(), SIGSEGV);
        case SIGINT:
        case SIGTERM:
            signal_abortSimulation(sigVal);
            break;
        default:
            break;
    }
}

void signal_abortSimulation(int sigVal) {
    log_debug(10, "Received signal %d. Aborting simulation: Killing %d children", sigVal, processCount);
    for (int i = 0; i < processCount; i++) {
        log_debug(10, "Killing PID: %d", childPid[i]);
        kill(childPid[i], SIGUSR1);
    }
    logger_end();
    view_end();
    exit(1);
}

void signal_createHandlerThread(int isMainProcess) {
    sigset_t signal_set;
    pthread_t signal_thread;
    int *param = malloc(sizeof(int));
    *param = isMainProcess;
    sigfillset(&signal_set);
    pthread_sigmask(SIG_BLOCK, &signal_set, NULL);

    if (!!pthread_create(&signal_thread, NULL, signal_threadHandler, param)) {
        log_error("Error creating signal thread");
    }   
}

void *signal_threadHandler(void* arg) {
    sigset_t signal_set;
    int sig;
    int isMainProcess = *((int*)arg);
    free(arg);
    while (TRUE) {
        sigfillset(&signal_set);
        sigwait(&signal_set, &sig);
        if (isMainProcess == TRUE) {
            signal_handler(sig);
        } else {
            signal_childHandler(sig);
        }
    }
    return NULL;
}

void signal_childHandler(int sigVal) {
    if (sigVal == SIGUSR1) {
        signal_endChildProcess(sigVal);
        return;
    }
    kill(getppid(), sigVal);
    return;
}

void signal_endChildProcess(int sigVal) {
    company_closeIpc();
    _exit(1);
}

