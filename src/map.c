#include "map.h"

Map *map_initialize(Map* map, int maxCityCount) {
	map->city = malloc(sizeof(City*) * maxCityCount);
	map->cityCount = maxCityCount;
	map->cityDistance = malloc(sizeof(int*) * maxCityCount);
	map->cityRoute = malloc(sizeof(int*) * maxCityCount);
	for (int i = 0; i < maxCityCount; ++i) {
		map->cityDistance[i] = malloc(sizeof(int) * maxCityCount);
		map->cityRoute[i] = malloc(sizeof(int) * maxCityCount);
		for (int j = 0; j < maxCityCount; ++j) {
			map->cityDistance[i][j] = 0;
			map->cityRoute[i][j] = 0;
			if (i == j) {
				map->cityRoute[i][j] = -1;
			}
		}
	}
	return map;
}

void map_setCity(Map *map, City *city, int index) {
	if (0 < index && index < map->cityCount) {
		map->city[index] = city;
	} else {
		log_error("Trying to append city to invalid index, %d", index);
	}
}

int map_getCityId(Map *map, char *name) {
	for (int i = 0; map->cityCount; i++) {
		if (!strcmp(map->city[i]->name, name)) {
			return i;
		}
	}
	log_error("Invalid city name %s", name);
	return -1;
}

void map_update(Map* map, CityUpdatePackage* update) {
	map->city[update->cityId]->itemStock[update->itemId] += update->amount;
}

// ====================================
//	functions to work with cities
// ====================================

City *newCity(char* name) {
	City* city = malloc(sizeof(City));
	city->id = -1;
	int len = strlen(name);
	memcpy(city->name, name, len);
	city->itemStock = NULL;
	city->itemCount = 0;
	return city;
}

