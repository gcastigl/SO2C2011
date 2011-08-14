#include "../include/map.h"

#define CITIES	3

Map* createMap() {
	Map * map = (Map*) malloc(sizeof(Map));
	map->distances = createFloatMatrix(CITIES, CITIES);
	int i, j;
	for (i = 0; i < CITIES; i++) {
		for (j = 0; j < CITIES; j++) {
			if (i == j) {
				map->distances[i][j] = 0;
			} else if (i < j) {
				map->distances[i][j] = random() % 20;
			} else {
				map->distances[i][j] = map->distances[j][i]; 
			}
		}
	}
	return map;
}

