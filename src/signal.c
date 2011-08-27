#include "../include/signal.h"

void signalHandler(int signal);
void childSignalHandler(int signal);

void initSignalHandler() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
}

void signalHandler(int sigVal) {
    switch (sigVal) {
        case SIGINT:
            //Do something such as die with honor :D
            break;
        case SIGTERM:
            //Do something such as die with honor :D
        default:
            break;
    }
}

void initChildSignalHandler() {
    signal(SIGINT, childSignalHandler);
    signal(SIGTERM, childSignalHandler);
}

void childSignalHandler(int sigVal) {
    signal(sigVal, childSignalHandler);
    kill(getppid(), sigVal);
}