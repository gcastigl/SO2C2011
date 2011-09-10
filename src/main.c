#include "main.h"


void initEnvironment();
void initializeServer();
void initializeCompanies();
void endSimulation();

static int processCount;
static Server server;
static Map map;

/*
 * 1 - Initialize Environment
 * 2 - Initialize Server
 * 3 - Initialize Companies
 * 4 - StartServer
 * 5 - EndSimulation
 */
int main() {
	log_debug(10, "Starting simulation...\n");
    initEnvironment();
    initializeServer();
    initializeCompanies();
    server_start(&server, &map);
    endSimulation();
	return 0;
}
/*
 *	Parses the cities, initializes companies and starts the signal handler.
 */
void initEnvironment() {
    signal_createHandlerThread(TRUE);
    parser_parseCitiesFile("./resources/loads/", &server, &map);
    parser_parseCompanies("./resources/loads/companies/", &server, &map);
    processCount = server.companyCount;
    signal_setProcessCount(processCount);
    childPid = malloc(sizeof(int) * (processCount));
    view_start();
    view_renderMap(&server, &map);
    log_debug(10, "[Main] Cities & companies parsed correctly");
}

/*
 * Cleans all ipc's queues and initializes all semaphores.
 */
void initializeServer() {
	// Initialize server semaphore.
	int semId = semaphore_create(SERVER_SEM_KEY, server.companyCount + 1, SEM_FLAGS);
	ipc_close(SERVER_IPC_KEY);
	log_debug(10, "[Main] Initialized semaphore for server (key = %d)", semId);
	for (int i = 0; i < server.companyCount; ++i) {
		semId = semaphore_create(server.company[i]->id, server.company[i]->planeCount + 1, SEM_FLAGS);
		ipc_close(server.company[i]->id + 1);
		log_debug(10, "[Main] Initialized semaphore for company %d (key = %d)", server.company[i]->id, semId);
	}
}

void initializeCompanies() {
    pid_t pId;
    for (int i = 0; i < server.companyCount; i++) {
        switch((pId = fork())) {
            case 0:
                signal_createHandlerThread(FALSE);
                companyStart(&map, server.company[i]);
                break;
            case ERROR:
                fatal("Fork Error");
                break;
            default:
                childPid[i] = pId;
                break;
        }
    }
    int serverSemId = semaphore_get(SERVER_SEM_KEY);
    for(int i = 0; i < server.companyCount; i++) {
    	// Wait for all companies to initialize...
    	semaphore_decrement(serverSemId, 0);
    }
    log_debug(10, "[Main] All companies were created correctly");
}

void endSimulation() {
    for (int i = 0; i < processCount; i++) {
        kill(childPid[i], SIGUSR1);
    }
    log_debug(10, "Simulation Done!");
    logger_end();
    view_end();
    exit(0);
}

