#ifndef MAPVIEW_H_
#define MAPVIEW_H_

#include <ncurses.h>
#include "map.h"
#include "server.h"

void view_start();
void view_end();

void view_renderMap(Server *server, Map *map);

#endif
