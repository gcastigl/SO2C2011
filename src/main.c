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
	log_debug("Starting simulation...\n");
    initEnvironment();
    /*initializeServer();
    initializeCompanies();
    server_start(server);
    endSimulation();*/
    printf("\n\nSimulation Done!\n\n");
	return 0;
}

void initEnvironment() {
    signal_createHandlerThread(TRUE);
    parser_parseCitiesFile("resources/loads/", &server, &map);
    log_debug("[Main] Cities file parsed correctly\n");
    parser_parseCompanies("resources/loads/", &server, &map);
    childPid = malloc(sizeof(int) * (processCount));
}
//TODO: initializer server
void initializeServer() {
	// server_addCompany(map, parseCompany(map, "resources/loads/empresa.txt", 1));
	processCount = server.companyCount;
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
}

void endSimulation() {
    for (int i = 0; i < processCount; i++) {
        kill(childPid[i], SIGUSR1);
    }
    log_debug("\n\nSimulation Done!\n\n");
    logger_end();
    printf("\n\nSimulation Done!\n\n");
    exit(0);
}

