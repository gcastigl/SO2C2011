#include "util/signalHandling.h"

void initSignalHandler() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
}

void signalHandler(int sigVal) {
    switch (sigVal) {
        case SIGINT:
        case SIGTERM:
            finishMainProcess(sigVal);
        default:
            break;
    }
}

void finishMainProcess(int sigVal) {
    for (int i = 0; i <= map->companyCount; i++) {
        kill(childPid[i], SIGUSR1);
    }
    endLogging();
}

void createSignalHandlingThread() {
    sigset_t signal_set;
    pthread_t sig_thread;
    
    sigfillset(&signal_set);
    pthread_sigmask(SIG_BLOCK, &signal_set, NULL);
    
    log_debug("Init signal handler thread\n");
    if (!!pthread_create(&sig_thread, NULL, sig_threadHandler, NULL)) {
        log_error("Error creating signal thread");
    }   
}

void *sig_threadHandler(void* args) {
    sigset_t signal_set;
    int sig;
    while (TRUE) {
        sigfillset(&signal_set);
        sigwait(&signal_set, &sig);
        log_debug("Signal thead on PID %d caught signal number %d\n", getpid(), sig);
        childSignalHandler(sig);
    }
    return NULL;
}

void childSignalHandler(int sigVal) {
    if (sigVal == SIGUSR1) {
        finishProcess(sigVal);
        return;
    }
    signal(sigVal, childSignalHandler);
    kill(getppid(), sigVal);
    return;
}

void finishProcess(int sigVal) {
    _exit(1);
}

