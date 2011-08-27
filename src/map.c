#include "../include/map.h"

#define CITIES	3

void createCities(Map* map);

Map* createRandomMap() {
	Map* map = malloc(sizeof(Map));
	createCities(map);
	map->turnCount = 0;
	return map;
}

void createCities(Map* map) {
	map->cities = malloc(sizeof(City) * CITIES);
	map->citiesSize = CITIES;
	int i, j;
	for(i = 0; i < map->citiesSize; i++) {
		City* c = createRandomCity(i);
		map->cities[i] = *c;
	}
	// Random distances between all cities...
	map->distances = createIntMatrix(map->citiesSize, map->citiesSize);
	for (i = 0; i < map->citiesSize; i++) {
		for (j = 0; j < map->citiesSize; j++) {
			if (i == j) {
				map->distances[i][j] = 0;
			} else if (i < j) {
				map->distances[i][j] = randomInt(10, 25);
			} else {
				map->distances[i][j] = map->distances[j][i];
			}
		}
	}
}

City* createRandomCity(int id) {
	int i;
	City* city = malloc(sizeof(City));
	city->id = id;
	city->needsSize = randomInt(1, 10);
	city->needs = malloc(sizeof(Item) * city->needsSize);
	for (i = 0; i < city->needsSize; i++) {
		Item * item = createRandomItem(i);
		city->needs[i] = *item;
	}
	return city;
}

Item* crateRandomItemArray(int* dim) {
	*dim = randomInt(3, 8);
	Item* items = malloc(sizeof(Item) * (*dim));
	int i;
	for (i = 0; i < *dim; ++i) {
		Item *it = createRandomItem(i);
		items[i] = *it;
	}
	return items;
}

Item* createRandomItem(int id) {
	Item* item = malloc(sizeof(Item));
	item->id = id;
	item->amount = randomInt(3, 10);
	return item;
}
