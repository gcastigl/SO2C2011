#include "../include/map.h"

void map_init(int maxCityCount) {
	map = malloc(sizeof(Map));
	map->cityCount = 0;
	map->itemCount = 0;
	map->itemName = malloc(100*sizeof(char*));
	map->city = malloc(maxCityCount * sizeof(City));
}

City *newCity(char* name) {
	City* city = malloc(sizeof(City));
	city->name = name;
	city->itemStock = malloc(100*sizeof(int));
	return city;
}

void map_addCity(City city) {
	map->city[map->cityCount++] = city;
}

int map_getStockId(char* name) {
	int i;
	int x = map->itemCount;
	for(i = 0; i < map->itemCount; i++) {
		if (!strcmp(map->itemName[i], name)) {
			return i;
		}
	}
	map->itemCount++;
	map->itemName[i] = name;
	return i;
}
