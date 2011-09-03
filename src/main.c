#include "main.h"

void startSimulation();
void initEnvironment();

int main() {
    startSimulation();
    printf("Simulation Done!\n");
	return 0;
}

void startSimulation() {
    initEnvironment();
    pid_t pId;
    initSignalHandler();
    for (int i = 0; i < map->companyCount; i++) {
        switch(pId = fork()) {
            case 0:
                initChildSignalHandler();
                companyStart(map->company[0]);
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
}

void initEnvironment() {
    parseMap("resources/loads/ciudades.txt");
    map_addCompany(parseCompany("resources/loads/empresa.txt", 123456));
    childPid = malloc(sizeof(int) * map->companyCount);
    return;
}


