#ifndef SERVER_H_
#define SERVER_H_

#include "common.h"
#include "map.h"
#include <sys/wait.h>

typedef struct {
	int turn;
	char **itemName;
	int itemCount;
	Company **company;
	int companyCount;
} Server;

void server_start(Server* server);

int server_getItemId(Server *server, char* itemName);

#endif
