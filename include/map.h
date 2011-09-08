#ifndef MAP_H_
#define MAP_H_

#include "common.h"
#include "serializer.h"

typedef struct {
	int id;
	int cityIdFrom;
	int cityIdTo;
	int distanceLeft;
	int itemCount;
	int *itemStock;
} Plane;

typedef struct {
	int id;
	char name[MAX_NAME_LENGTH];
	int itemCount;
	int *itemStock;
} City;

typedef struct {
	int id;
	char name[MAX_NAME_LENGTH];
	int planeCount;
	Plane **plane;
} Company;

typedef struct {
	int cityCount;
	int **cityDistance;
	City **city;
} Map;

typedef struct {
	int turn;
	char **itemName;
	int itemCount;
	Company **company;
	int companyCount;
} Server;

Map *map_initialize(Map* map, int maxCityCount);

void map_setCity(Map *map, City *city, int index);

int map_getCityId(Map *map, char* name);

void map_update(Map* map, CityUpdatePackage* update);

City *newCity(char* name);

#endif

