#ifndef MAP_H_
#define MAP_H_

#include "common.h"
#include <pthread.h>

#define MAX_CITY_COUNT 64
#define MAX_ITEM_COUNT 128
#define MAX_PLANE_COUNT 32

typedef struct {
	pthread_t thread;
	int originCityIndex;
	int destinationCityIndex;
	int distanceToDestination;
	int itemStock[MAX_ITEM_COUNT];
	int id;
} Plane;

typedef struct {
	char *name;
	Plane plane[MAX_PLANE_COUNT];
	int planeCount;
} Company;

typedef struct {
	char *name;
	int itemStock[MAX_ITEM_COUNT];
	int cityDistance[MAX_CITY_COUNT];
} City;

typedef struct {
	int turn;
	int cityCount;
	City city[MAX_CITY_COUNT];
	int itemCount;
	char *itemName[MAX_ITEM_COUNT];
} Map;

void map_init(int maxCityCount);
City *newCity(char* name);
void map_addCity(City city);
int map_getCityId(char* name);
int map_getStockId(char* name);

Map *map;

#endif

