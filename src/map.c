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

void addCity(Map* map, City city) {
	map->city[map->cityCount++] = city;
}
