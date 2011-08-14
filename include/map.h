#ifndef _MAP_
#define _MAP_

#include "common.h"
#include <string.h>

typedef struct {
	int id;
	char* name;
	Item* needs;
} City;

typedef struct {
	City* cities;
	float** distances;
	int planeCount;
	int turnCount;
} Map;

Map* createMap();

#endif
