#ifndef _MAP_
#define _MAP_

#include "common.h"

typedef struct {
	int id;
	Item* needs;
	int needsSize;
} City;

typedef struct {
	City* cities;
	int citiesSize;
	int** distances;
	int planeCount;
	int turnCount;
} Map;

Map* createMap();

#endif

