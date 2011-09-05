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
	int ownerCompanyId;
	pthread_t thread;
	int cityIdFrom;
	int cityIdTo;
	int distanceToDestination;
	int itemStock[MAX_ITEM_COUNT];
} Plane;

typedef struct {
	int id;
	char *name;
	Plane *plane[MAX_PLANE_COUNT];
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
	City *city[MAX_CITY_COUNT];
	int itemCount;
	char *itemName[MAX_ITEM_COUNT];
	int companyCount;
	Company *company[MAX_COMPANY_COUNT];
} Map;

Map *newMap();
City *newCity(char* name);
void map_addCity(Map *map, City *city);
void map_addCompany(Map *map, Company *company);
int map_getCityId(Map *map, char* name);
int map_getStockId(Map *map, char* name);

#endif

