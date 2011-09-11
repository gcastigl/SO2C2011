#ifndef SERVER_H_
#define SERVER_H_

#include "common.h"
#include "communicator.h"
#include "map.h"
#include <sys/wait.h>
#include <time.h>
#include "view/mapview.h"
//#include <features.h>

#define REFRESH_TIME_SECONDS	0.1

void server_start(Server* server, Map* initialMap);

int server_getItemId(Server *server, char* itemName);

#endif
