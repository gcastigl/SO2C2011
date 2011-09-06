#include "map.h"

Map *newMap() {
	Map *map = malloc(sizeof(Map));
	map->cityCount = 0;
	// TODO: for the distances matrix, make the array dinamic and make a malloc
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

int map_getCityId(Map *map, char *name) {
	for (int i = 0; map->cityCount; i++) {
		if (!strcmp(map->city[i]->name, name)) {
			return i;
		}
	}
	fatal("Invalid city id");
	return 0;
}

int map_start(Map *map) {
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
