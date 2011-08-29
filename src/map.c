#include "map.h"

void map_init(int maxCityCount) {
	map = malloc(sizeof(Map));
	map->cityCount = 0;
	map->itemCount = 0;
}

City *newCity(char* name) {
	City* city = malloc(sizeof(City));
	city->name = malloc((strlen(name)+1) * sizeof(char));
	strcpy(city->name, name);
	return city;
}

void map_addCity(City city) {
	map->city[map->cityCount++] = city;
}

int map_getCityId(char *name) {
	for (int i = 0; map->cityCount; i++) {
		if (!strcmp(map->city[i].name, name)) {
			return i;
		}
	}
}

int map_getStockId(char* name) {
	int i;
	for(i = 0; i < map->itemCount; i++) {
		if (!strcmp(map->itemName[i], name)) {
			return i;
		}
	}
	map->itemCount++;
	map->itemName[i] = malloc((strlen(name)+1) * sizeof(char));
	strcpy(map->itemName[i], name);
	return i;
}
