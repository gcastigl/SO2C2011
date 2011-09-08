#include "server.h"

void broadcastUpdateMessages(Server* server);

static int activeCompanies;

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
	for(int i = 0; i < 10; ++i) {
		// FIXME: when all companies die, the server stays locked forever in the semaphore.
		// FIX: When update packages get finished, see companyLogic(bit uage for living planes) and do the same thing here.
		server->turn++;
		log_debug("------------------------TURN %d--------------------------", server->turn);
		for(int j = 0; j < server->companyCount; ++j) {
			log_debug("[Server] Company %d plays turn %i", j, i);
			//Give each company one turn...
			semaphore_increment(semId, j + 1);
			semaphore_decrement(semId, 0);
			broadcastUpdateMessages(server);
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

/*
 * 1 - Read all messages
 * 2 - if message = kill company => free that memory segment
 * 3 - if message = updateCity => send that update to all OTHER companies & clear queue.
 */
void broadcastUpdateMessages(Server* server) {
	char msg[MAX_NAME_LENGTH];
	for (int i = 0; i < server->companyCount; ++i) {
		log_debug("company: %d\n", server->company[i]->id);
		ipc_read(SERVER_IPC_KEY, server->company[i]->id + 1, msg);
	}
	log_debug("This is a broad cast!!\n");
}
