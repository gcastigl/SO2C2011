#include "map.h"

void map_init(int maxCityCount) {
	map = malloc(sizeof(Map));
	map->cityCount = 0;
	map->itemCount = 0;
}

City *newCity(char* name) {
	City* city = malloc(sizeof(City));
	mstrcpy(&(city->name), name);
	return city;
}

void map_addCity(City city) {
	map->city[map->cityCount++] = city;
}

void map_addCompany(Company company) {
	map->company[map->companyCount++] = company;
}

int map_getCityId(char *name) {
	for (int i = 0; map->cityCount; i++) {
		if (!strcmp(map->city[i].name, name)) {
			return i;
		}
	}
	fatal("Invalid city id");
	return 0;
}

int map_getStockId(char* name) {
	int i;
	for(i = 0; i < map->itemCount; i++) {
		if (!strcmp(map->itemName[i], name)) {
			return i;
		}
	}
	mstrcpy(&(map->itemName[map->itemCount++]), name);
	return i;
}
