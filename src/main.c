#include "main.h"

void startSimulation();
void initEnvironment();
void startSimulationDisplayer();

int main() {
    initEnvironment();
    startSimulationDisplayer();
    startSimulation();
    printf("Simulation Done!\n");
	return 0;
}

void startSimulation() {
    pid_t pId;
    // Initialize companies
    for (int i = 0; i < map->companyCount; i++) {
        switch(pId = fork()) {
            case 0:
                initChildSignalHandler();
                companyStart(map->company[i]);
                exit(0);
                break;
            case ERROR:
                fatal("Fork Error");
                break;
            default:
                childPid[i] = pId;
                break;
        }
    }
    wait(NULL);
    printf("\n\nSimulation Done!\n\n\n");
}

void initEnvironment() {
    initSignalHandler();
    parseMap("resources/loads/ciudades.txt");
    map_addCompany(parseCompany("resources/loads/empresa.txt", 123456));
    childPid = malloc(sizeof(int) * map->companyCount + 1);
    return;
}

void startSimulationDisplayer() {
    pid_t pId;
    pId = fork();
    if (pId == 0) {
        //initChildSignalHandler();
        displaySimulation();
    } else if (pId == ERROR) {
        fatal("Error forking UI");
    } else {
        childPid[map->companyCount + 1] = pId;
    }
}
