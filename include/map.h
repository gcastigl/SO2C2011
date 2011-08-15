#ifndef _MAP_
#define _MAP_

#include "common.h"

typedef struct {
	int id;
	int needsCount;
	Item* needs;
} City;

typedef struct {
	City* cities;
	int citiesCount;
	int** distances;
	int planeCount;
	int turnCount;
} Map;

Map* createMap();

#endif
