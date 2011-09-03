#include "util/signal.h"

void signalHandler(int signal);
void childSignalHandler(int signal);
void finishProcess(int sigVal);
void finishMainProcess(int sigVal);

void initSignalHandler() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
}

void signalHandler(int sigVal) {
    log_debug("Main process received signal %d\n", sigVal);
    switch (sigVal) {
        case SIGINT:
        case SIGTERM:
            finishMainProcess(sigVal);
        default:
            break;
    }
}

void finishMainProcess(int sigVal) {
    log_debug("Sending SIGUSR1 to %d child%s\n", map->companyCount, map->companyCount > 1  ? "s" : "");
    for (int i = 0; i <= map->companyCount; i++) {
        kill(childPid[i], SIGUSR1);
    }
}

void initChildSignalHandler() {
    signal(SIGINT, childSignalHandler);
    signal(SIGTERM, childSignalHandler);
    signal(SIGUSR1, childSignalHandler);
}

void childSignalHandler(int sigVal) {
    if (sigVal == SIGUSR1) {
        log_debug("Child received sig SIGUSR1, time to wrap up!\n");
        finishProcess(sigVal);
        return;
    }
    signal(sigVal, childSignalHandler);
    log_debug("Sending signal %d to PID: %d\n", sigVal, getppid());
    kill(getppid(), sigVal);
    return;
}

void finishProcess(int sigVal) {
    exit(1);
}