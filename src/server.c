#include "server.h"

Server* newServer(int maxCompanyCount) {
	Server* server = malloc(sizeof(Server));
	server->company = malloc(sizeof(Company*) * maxCompanyCount);
	server->companyCount = 0;
	server->itemCount = 0;
	server->itemName = NULL;
	return server;
}

void server_start(Server* server) {
    for (int i = 0; i < server->companyCount; ++i) {
        wait(NULL);
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
 *
 *
 *
 *
int map_start(Map *mapArg) {
    int semId;
	pthread_t display_thread;

	map = mapArg;
	semId = semaphore_create(MAP_SEM_KEY, 1, 0666);
	semctl(semId, 0, SETVAL, 1);
	if (semId == -1) {
		fatal("Error creating map semaphore");
	}
	if (!!pthread_create(&display_thread, NULL, map_service, NULL)) {
		fatal("Error creating display thread");
	}
	return TRUE;
}

void *map_service(void *args) {

	while(1) {
		// Should read and wait for info, and later send info
		sleep(1);
	}
	return NULL;
}
 */
