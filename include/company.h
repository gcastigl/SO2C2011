#ifndef _AIRLINE_
#define _AIRLINE_

#include "common.h"
#include "plane.h"
#include "ipc.h"

Company* createAirline(long id, Map* map, int numberOfPlanes);

void airlineStart(Company* company);

#endif

