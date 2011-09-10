#include "server.h"

void server_readMessages(Server* server, int companyNumber);
void server_broadcastUpdateMessage(Server* server, int fromCompanyId, CityUpdatePackage *update);
void server_applyMapUpdate(CityUpdatePackage* cityUpdate);
static int activeCompanies;
static Map *serverMap;
Server* newServer(int maxCompanyCount) {
	Server* server = malloc(sizeof(Server));
	server->company = malloc(sizeof(Company*) * maxCompanyCount);
	server->companyCount = 0;
	server->itemCount = 0;
	server->itemName = NULL;
	return server;
}

void server_start(Server* server, Map* initialMap) {
	int semId = semaphore_get(SERVER_SEM_KEY);
	activeCompanies = (1 << server->companyCount) - 1;
	time_t  currTime, lastUpdate = -1;
    serverMap = initialMap;
	while(TRUE) {
		// FIXME: when all companies die, the server stays locked forever in the semaphore.
		// FIX: When update packages get finished, see companyLogic(bit uage for living planes) and do the same thing here.
		server->turn++;
		log_debug(10, "------------------------TURN %d--------------------------", server->turn);
		for(int j = 0; j < server->companyCount; ++j) {
			if (activeCompanies & (1 << j)) { // if company i is active
				log_debug(0, "[Server] Company %d plays turn %i", j, server->turn);
				//Give each company one turn...
				semaphore_increment(semId, j + 1);
				semaphore_decrement(semId, 0);
				server_readMessages(server, server->company[j]->id);
			}
		}
		currTime = time(NULL);
		if (lastUpdate == -1 || currTime - lastUpdate > REFRESH_TIME_SECONDS) {
			view_renderMap(server, serverMap);
			lastUpdate = time(NULL);
		}
	}
}

int server_getItemId(Server *server, char* itemName) {
	if (server->itemName == NULL) {
		return -1;
	}
	for(int i = 0; i < server->itemCount; i++) {
		if (!strcmp(server->itemName[i], itemName)) {
			return i;
		}
	}
	return -1;
}

void server_readMessages(Server* server, int fromCompanyId) {
    CompanyUpdatePackage* companyUpdate;
    CityUpdatePackage* cityUpdate;
	int packageType;
	void* package;
	do {
		package = serializer_read(SERVER_IPC_KEY, fromCompanyId + 1, &packageType);
		if (package != NULL) {
			log_debug(5, "A package type= %d has been read from the serializer by the server", packageType);
			switch(packageType) {
				case PACKAGE_TYPE_COMPANY:
					free(server->company[fromCompanyId]); // Memory still allocated for planes
					server->company[fromCompanyId] = (Company*) package;
					break;
				case PACKAGE_TYPE_COMPANY_UPDATE: // Turn off bit i from activeCompanies
					companyUpdate = (CompanyUpdatePackage*) package;
					if (companyUpdate->status == OFF) {
					    activeCompanies &= ~(1 << companyUpdate->companyId);
					}
					break;
				case PACKAGE_TYPE_CITY_UPDATE:
                    cityUpdate = (CityUpdatePackage*) package;
                    server_applyMapUpdate(cityUpdate);
                    server_broadcastUpdateMessage(server, fromCompanyId, cityUpdate);
					break;
				default:
					log_error("the server received an unknown package type: %d", packageType);
			}
		}
	} while (package != NULL);
}

void server_applyMapUpdate(CityUpdatePackage* cityUpdate) {
    
}

/*
 * 1 - Read all messages
 * 2 - if message = kill company => free that memory segment
 * 3 - if message = updateCity => send that update to all OTHER companies & clear queue.
 */
void server_broadcastUpdateMessage(Server* server, int fromCompanyId, CityUpdatePackage* update) {
    Company* company;
    for (int i = 0; i < server->companyCount; i++) {
        company = server->company[i];
        if (company->id != fromCompanyId) {
            serializer_write_cityUpdate(update, SERVER_IPC_KEY, company->id + 1);
        }
    }
}
