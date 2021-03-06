#include "server.h"

void server_readMessages(Server* server, int companyNumber);
void server_broadcastUpdateMessage(Server* server, int fromCompanyId, CityUpdatePackage *update);
char *getCompanySemName(char* buffer, int index, Server* s);
static int activeCompanies;
static Map *serverMap;
#define IS_ACTIVE(X) ((activeCompanies >> X) & 1)

Server* newServer(int maxCompanyCount) {
	Server* server = malloc(sizeof(Server));
	server->company = malloc(sizeof(Company*) * maxCompanyCount);
	server->companyCount = 0;
	server->itemCount = 0;
    server->turn = 0;
	server->itemName = NULL;
	return server;
}

void server_start(Server* server, Map* initialMap) {
    char semName[10];
	activeCompanies = (1 << server->companyCount) - 1;
	time_t  currTime, lastUpdate = -1;
    serverMap = initialMap;
    
    for (int a = 0; a < server->companyCount; a++)
        server_readMessages(server, server->company[a]->id);
    
	while(activeCompanies != 0) {
		server->turn++;
		log_debug("-----------------------Turn %d-----------------------", server->turn);
		usleep(700 * 1000);
		for(int j = 0; j < server->companyCount; ++j) {
		    server_readMessages(server, server->company[j]->id);
			if (IS_ACTIVE(j)) { // if company i is active
				//Give each company one turn...
                S_POST(getCompanySemName(semName, j, server));
                S_WAIT("server");
			}
			server_readMessages(server, server->company[j]->id);
		}
		currTime = time(NULL);
		if (lastUpdate == -1 || currTime - lastUpdate > REFRESH_TIME_SECONDS) {
			view_renderMap(server, serverMap);
			lastUpdate = time(NULL);
		}
	}
	view_renderMap(server, serverMap);
}

char* getCompanySemName(char* buffer, int index, Server* server) {
    sprintf(buffer, "c%d", server->company[index]->id);
    return buffer;
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
		package = serializer_read(serverId, fromCompanyId + 1, &packageType);
		if (package != NULL) {
			switch(packageType) {
				case PACKAGE_TYPE_COMPANY:
					free(server->company[fromCompanyId]); // Memory still allocated for planes
					server->company[fromCompanyId] = (Company*) package;
                    log_debug("Company %d structure update", server->company[fromCompanyId]->id);
					break;
				case PACKAGE_TYPE_COMPANY_UPDATE: // Turn off bit i from activeCompanies
					companyUpdate = (CompanyUpdatePackage*) package;
                    log_debug("Company %d update", companyUpdate->companyId);
					if (companyUpdate->status == OFF) {
					    activeCompanies &= ~(1 << companyUpdate->companyId);
					}
					break;
				case PACKAGE_TYPE_CITY_UPDATE:
                    cityUpdate = (CityUpdatePackage*) package;
                    log_debug("City Update");
                    map_update(serverMap, cityUpdate);
                    server_broadcastUpdateMessage(server, fromCompanyId, cityUpdate);
					break;
				default:
					log_error("the server received an unknown package type: %d", packageType);
			}
		}
	} while (package != NULL);
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
            serializer_write(update, serverId, company->id + 1, PACKAGE_TYPE_CITY_UPDATE);
        }
    }
}
