#ifndef _MAP_
#define _MAP_

#include "common.h"
#include <string.h>

typedef struct {
	char* name;
	int id;
	Item* needs;
} City;

typedef struct {
	City* cities;
	float** distances;
} Map;

Map* createMap();

#endif
