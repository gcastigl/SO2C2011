#ifndef MAP_H_
#define MAP_H_

#include "common.h"
#include <pthread.h>

//FIXME: remove all this values
#define MAX_CITY_COUNT 64
#define MAX_COMPANY_COUNT 64
#define MAX_ITEM_COUNT 128
#define MAX_PLANE_COUNT 32

typedef struct {
	int id;
	int cityIdFrom;
	int cityIdTo;
	int distanceLeft;
	int itemCount;
	int itemStock[MAX_ITEM_COUNT];
} Plane;

typedef struct {
	int id;
	char *name;
	int itemCount;
	int itemStock[MAX_ITEM_COUNT];
} City;

typedef struct {
	int id;
	char *name;
	int planeCount;
	Plane *plane[MAX_PLANE_COUNT];
} Company;

typedef struct {
	int cityCount;
	int cityDistance[MAX_CITY_COUNT][MAX_CITY_COUNT];
	City *city[MAX_CITY_COUNT];
} Map;

typedef struct {
	int turn;
	int companyCount;
	char *itemName[MAX_ITEM_COUNT];
	Company *company[MAX_COMPANY_COUNT];
} Server;

Map *newMap();

City *newCity(char* name);

void map_addCity(Map *map, City *city);

int map_getCityId(Map *map, char* name);

int map_start(Map *map);

#endif

