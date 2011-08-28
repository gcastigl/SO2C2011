#include "../include/map.h"

Map *newMap(int maxCityCount) {
	Map* map = malloc(sizeof(Map));
	map->cityCount = 0;
	map->city = malloc(maxCityCount * sizeof(City));
	return map;
}

City *newCity(char* name) {
	City* city = malloc(sizeof(City));
	city->name = name;
	return city;
}

void map_addCity(City city) {
	map->city[map->cityCount++] = city;
}

int map_getStockId(char* name) {
	int i;
	for(i = 0; i<map->itemCount; i++) {
		if (!strcmp(map->itemName[i], name)) {
			return i;
		}
	}
	map->itemCount++;
	map->itemName[i] = name;
	return i;
}
