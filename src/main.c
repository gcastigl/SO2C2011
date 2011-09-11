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
}

/*
 * Cleans all ipc's queues and initializes all semaphores.
 */
void initializeServer() {
	// Initialize server semaphore.
    log_debug("Initializing simulation");
	int semId = semaphore_create(SERVER_SEM_KEY, server.companyCount + 1, SEM_FLAGS);
	ipc_init(SERVER_IPC_KEY, 0);
	for (int i = 0; i < server.companyCount; ++i) {
		semId = semaphore_create(server.company[i]->id, server.company[i]->planeCount + 1, SEM_FLAGS);
		if (semId < 0) {
            log_error("Error creating semaphore for company %d", server.company[i]->id);
		}
		ipc_init(server.company[i]->id + 1, 0);
	}
}

void initializeCompanies() {
    pid_t pId;
    for (int i = 0; i < server.companyCount; i++) {
        switch((pId = fork())) {
            case 0:
                signal_createHandlerThread(FALSE);
                companyStart(&map, server.company[i]);
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
    int serverSemId = semaphore_get(SERVER_SEM_KEY);
    for(int i = 0; i < server.companyCount; i++) {
    	// Wait for all companies to initialize...
    	semaphore_decrement(serverSemId, 0);
    }
}

void endSimulation() {
    for (int i = 0; i < processCount; i++) {
        kill(childPid[i], SIGUSR1);
    }
    logger_end();
    view_end();
    exit(0);
}

