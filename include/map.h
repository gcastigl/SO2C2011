#ifndef MAP_H_
#define MAP_H_

#include "common.h"
#include <pthread.h>

typedef struct {
	pthread_t thread;
	int originCityIndex;
	int destinationCityIndex;
	int distanceToDestination;
	int itemStock[];
} Plane;

typedef struct {
	char* name;
	Plane* plane;
	int planeCount;
} Company;

typedef struct {
	char* name;
	int* itemStock;
	int cityDistance[];
} City;

typedef struct {
	int turn;
	int cityCount;
	City* city;
	int itemCount;
	char* itemName[];
} Map;

Map * newMap(int maxCityCount);
void addCity(Map* map, City city);

#endif

