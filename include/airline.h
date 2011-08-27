#ifndef AIRLINE_H_
#define AIRLINE_H_

#include "common.h"
#include "plane.h"
#include "ipc.h"

Company* createAirline(long id, Map* map, int numberOfPlanes);

void airlineStart(Company* company);

#endif

