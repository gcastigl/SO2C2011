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
        switch((pId = fork())) {
            case 0:
                signal_createHandlerThread(FALSE);
                log_debug("CREATED COMPANY WITH PID %d\n", getpid());
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
    for (int i = 0; i <= map->companyCount; ++i) {
        wait(NULL);
    }
}

void initEnvironment() {
    signal_createHandlerThread(TRUE);
    parseMap("resources/loads/ciudades.txt");
    map_addCompany(parseCompany("resources/loads/empresa.txt", 123456));
    childPid = malloc(sizeof(int) * (map->companyCount + 1));
    return;
}

void startSimulationDisplayer() {
    pid_t pId;
    switch ((pId = fork())) {
        case 0:
            log_debug("CREATED DISPLAY WITH PID %d\n", getpid());
            signal_createHandlerThread(FALSE);
            displaySimulation();
            break;
        case ERROR:
            fatal("Error forking UI");
            break;
        default:
            childPid[map->companyCount] = pId;
            break;
    }
}
