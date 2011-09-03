#include "main.h"

void startSimulation();
void initEnvironment();
void startSimulationDisplayer();

int main() {
    startSimulation();
    printf("Simulation Done!\n");
	return 0;
}

void startSimulation() {
    initEnvironment();
    pid_t pId;
    initSignalHandler();
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
    // Initialize UI thread
	/* switch(pId = fork()) {
		case 0:
			startSimulationDisplayer();
			exit(0);
			break;
		case ERROR:
			fatal("Fork Error");
			break;
	}*/
    wait(NULL);
    printf("\n\nSimulation Done!\n\n\n");
}

void initEnvironment() {
    parseMap("resources/loads/ciudades.txt");
    map_addCompany(parseCompany("resources/loads/empresa.txt", 123456));
    childPid = malloc(sizeof(int) * map->companyCount);
    return;
}

void startSimulationDisplayer() {
	displaySimulation();
}
