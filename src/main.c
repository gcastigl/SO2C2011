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
    log_debug("Initializing simulation");
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
    char semName[10];
    int val;
	sem_t *sem = semaphore_create("server");
    S_GETVAL("server", &val);
    log_debug("server sem: ", val);
	ipc_init(SERVER_IPC_KEY, 0);
	for (int i = 0; i < server.companyCount; ++i) {
        sprintf(semName, "c%d", server.company[i]->id);
        sem = semaphore_create(semName);
		if (sem == SEM_FAILED) {
            log_error("Error creating semaphore for company %d", server.company[i]->id);
            fatal("");
		}
		ipc_init(server.company[i]->id + 1, 0);
	}
}

void initializeCompanies() {
    pid_t pId;
    sem_t* cr = semaphore_create("companyReady");
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
    for(int i = 0; i < server.companyCount; i++) {
    	// Wait for all companies to initialize...
    	sem_wait(cr);
    }
    sem_close(cr);
}

void endSimulation() {
    for (int i = 0; i < processCount; i++) {
        kill(childPid[i], SIGUSR1);
    }
    logger_end();
    view_end();
    exit(0);
}

