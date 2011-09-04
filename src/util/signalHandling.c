#include "util/signalHandling.h"

void signal_handler(int sigVal) {
    switch (sigVal) {
        case SIGINT:
        case SIGTERM:
            signal_abortSimulation(sigVal);
        default:
            break;
    }
}

void signal_abortSimulation(int sigVal) {
    for (int i = 0; i <= map->companyCount; i++) {
        kill(childPid[i], SIGUSR1);
    }
    endLogging();
}

void signal_createHandlerThread(int isMainProcess) {
    sigset_t signal_set;
    pthread_t signal_thread;
    int *param = malloc(sizeof(int));
    *param = isMainProcess;
    sigfillset(&signal_set);
    pthread_sigmask(SIG_BLOCK, &signal_set, NULL);

    log_debug("Init signal handler thread with parameter %s\n", *param == TRUE ? "TRUE" : "FALSE");
    if (!!pthread_create(&signal_thread, NULL, signal_threadHandler, param)) {
        log_error("Error creating signal thread");
    }   
}

void *signal_threadHandler(void* arg) {
    sigset_t signal_set;
    int sig;
    int isMainProcess = *((int*)arg);
    free(arg);
    log_debug("Creating signal handler for %s (argument: %d)\n", isMainProcess == TRUE ? "main" : "child", isMainProcess);
    while (TRUE) {
        sigfillset(&signal_set);
        sigwait(&signal_set, &sig);
        log_debug("Signal thead on PID %d caught signal number %d\n", getpid(), sig);
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
    _exit(1);
}

