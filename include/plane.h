#ifndef _PLANE_
#define _PLANE_

#include "common.h"
#include "map.h"
#include "mathUtil.h"

#define NO_TARGET	-1
#define SLEEP_TIME	1

typedef struct {
	int id;
	int originCityIndex;
	int destinationCityIndex;
	int distanceToDestination;
	Item* supplies;
	int suppliesSize;
	Map* map;
} Plane;

Plane* createPlane(Map* map, int id, int initialCityIndex, Item* supplies, int suppliesSize);

void planeStart(Plane* plane);

#endif

