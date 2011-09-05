#include "main.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

void startSimulation();
void initEnvironment();
void startMapAndDisplayProcess();
void main_endSimulation();
static Map *map;
static int processCount;

int main() {
    initEnvironment();
    startMapAndDisplayProcess();
    startSimulation();
    main_endSimulation();
	return 0;
}

void initEnvironment() {
    log_debug("Starting simulation...\n");
    map = parseMap("resources/loads/ciudades.txt");
    map_addCompany(map, parseCompany(map, "resources/loads/empresa.txt", 1));
    processCount = map->companyCount;
    signal_createHandlerThread(TRUE);
    childPid = malloc(sizeof(int) * (processCount));
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

// Also acts as a map information hub
void startMapAndDisplayProcess() {
	display_start(map);
	map_start(map);
}

void main_endSimulation() {
    for (int i = 0; i < processCount; i++) {
        kill(childPid[i], SIGUSR1);
    }
    logger_end();
    printf("\n\nSimulation Done!\n\n");
    exit(0);
}
