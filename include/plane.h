#ifndef PLANE_H_
#define PLANE_H_

#include "common.h"
#include "map.h"
#include "mathUtil.h"

#define NO_TARGET	-1

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

void* planeStart(void* param);

#endif

