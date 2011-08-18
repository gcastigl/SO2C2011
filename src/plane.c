#include "../include/plane.h"

void updateState(Map* map, Plane* plane);
int findBestCity(Map* map, Plane* plane);
int hasEnoughItems(Plane* plane, City* city);

Plane* createPlane(int id) {
    printf("Creating plane %d...\n", id);
	Plane* plane = (Plane*) malloc(sizeof(Plane));
	plane->id = id;
	plane->distanceLeft = -1;
	plane->targetId = NO_TARGET;
	plane->supplies = NULL;
	return plane;
}

void planeStart(Plane* plane) {
    printf("Plane %d created\nWaiting for data...\n", plane->id);
    ipcPackage data;
    data = getData(plane->id);
    printf("Data for plane %d: %s\n",plane->id, data.data);
	exit(0);
}

void updateState(Map* map, Plane* plane) {
	plane->distanceLeft--;
	if (plane->distanceLeft == 0) {
		int newTarget = findBestCity(map, plane);
		if (newTarget != NO_TARGET) {
			plane->distanceLeft = map->distances[plane->targetId][newTarget]; 	
			// Distance from currentTargetId to newTaget
			plane->targetId = newTarget;
		}
	}
}

int findBestCity(Map* map, Plane* plane) {
	int i;
	for (i = 0; i < map->citiesCount; i++) {
		City * currCity = &(map->cities[i]);
		if (hasEnoughItems(plane, currCity)) {
			return 	currCity->id;
		}
	}
	return NO_TARGET;
}

int hasEnoughItems(Plane* plane, City* city) {
	int i;
	for (i = 0; i < city->needsCount; i++) {
		int itemId = city->needs[i].id;
		if (city->needs[i].amount > plane->supplies[itemId].amount) {
			return FALSE;
		}
	}
	return TRUE;
}


