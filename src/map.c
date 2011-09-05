#include "map.h"

Map *newMap() {
	Map *map = malloc(sizeof(Map));
	map->cityCount = 0;
    map->companyCount = 0;
	map->itemCount = 0;
	return map;
}

City *newCity(char* name) {
	City* city = malloc(sizeof(City));
	mstrcpy(&(city->name), name);
	return city;
}

void map_addCity(Map *map, City *city) {
	map->city[map->cityCount++] = city;
}

void map_addCompany(Map *map, Company *company) {
	map->company[map->companyCount++] = company;
}

int map_getCityId(Map *map, char *name) {
	for (int i = 0; map->cityCount; i++) {
		if (!strcmp(map->city[i]->name, name)) {
			return i;
		}
	}
	fatal("Invalid city id");
	return 0;
}

int map_getStockId(Map *map, char* name) {
	int i;
	for(i = 0; i < map->itemCount; i++) {
		if (!strcmp(map->itemName[i], name)) {
			return i;
		}
	}
	mstrcpy(&(map->itemName[map->itemCount++]), name);
	return i;
}

int map_start() {
    int semId;
	semId = semaphore_create(MAP_SEM_KEY, 1, 0666);
	if (semId == -1) {
		fatal("Error creating map semaphore");
	}
	while(1) {
		// Should read and wait for info, and later send info
		sleep(1);
	}
}
