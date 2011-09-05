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
static pid_t uiPid;

int main() {
    initEnvironment();
    startMapAndDisplayProcess();
    startSimulation();
    printf("\n\nSimulation Done!\n\n");
    main_endSimulation();
	return 0;
}

void initEnvironment() {
    log_debug("Starting simulation...\n");
    signal_createHandlerThread(TRUE);
    parseMap("resources/loads/ciudades.txt");
    map_addCompany(parseCompany("resources/loads/empresa.txt", 1));
    childPid = malloc(sizeof(int) * (map->companyCount));
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
    int semId;
	signal_createHandlerThread(FALSE);
	semId = semaphore_create(MAP_SEM_KEY, 1, 0666);
	if (semId == -1) {
		fatal("Error creating map semaphore");
	}
	semctl(semId, 0, SETVAL, 1);
	displaySimulation();
}

void main_endSimulation() {
    for (int i = 0; i < map->companyCount; i++) {
        kill(childPid[i], SIGUSR1);
    }
    logger_end();
    exit(0);
}
