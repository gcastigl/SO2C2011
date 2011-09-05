#include "main.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

void startSimulation();
void initEnvironment();
void startSimulationDisplayer();
void main_endSimulation();
static pid_t uiPid;
static Map *map;

int main() {
    initEnvironment();
    // startSimulationDisplayer();
    startSimulation();
    printf("\n\nSimulation Done!\n\n");
	return 0;
}

void initEnvironment() {
    signal_createHandlerThread(TRUE);
    map = parseMap("resources/loads/ciudades.txt");
    map_addCompany(map, parseCompany(map, "resources/loads/empresa.txt", 1));
    childPid = malloc(sizeof(int) * (map->companyCount + 1));
    return;
}

void startSimulation() {
    pid_t pId;
    // Initialize companies
    for (int i = 0; i < map->companyCount; i++) {
        switch((pId = fork())) {
            case 0:
                signal_createHandlerThread(FALSE);
                companyStart(map->company[i]);
                break;
            case ERROR:
                fatal("Fork Error");
                break;
            default:
                childPid[i] = pId;
                break;
        }
    }
    for (int i = 0; i < map->companyCount; ++i) {
        wait(NULL);
    }
}

void startSimulationDisplayer() {
    pid_t pId;
    switch ((pId = fork())) {
        case 0:
            signal_createHandlerThread(FALSE);
            //displaySimulation();
            break;
        case ERROR:
            fatal("Error forking UI");
            break;
        default:
            childPid[map->companyCount] = uiPid = pId;
            break;
    }
}

void main_endSimulation() {
    kill(uiPid, SIGUSR1);
    for (int i = 0; i < map->companyCount; i++) {
        kill(childPid[i], SIGUSR1);
    }
    logger_end();
    exit(0);
}
