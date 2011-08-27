#ifndef MAP_H_
#define MAP_H_

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
	int turnCount;
} Map;

Map* createRandomMap();

City* createRandomCity(int id);

Item* crateRandomItemArray(int* dim);

Item* createRandomItem(int id);

#endif

