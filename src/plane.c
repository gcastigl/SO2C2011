#include "../include/plane.h"

void updateState(Map* map, Plane* plane);
int findBestCity(Map* map, Plane* plane);
int hasEnoughItems(Plane* plane, City* city);

Plane* createPlane(int id) {
	Plane* plane = (Plane*) malloc(sizeof(Plane));
	plane->id = id;
	plane->distanceLeft = -1;
	plane->targetId = NO_TARGET;
	plane->supplies = NULL;
	return plane;
}

void planeProcess(Plane* plane, int* wrPipe, int* rdPipe) {
	int count;
	char buf[2];
	close(wrPipe[READ]);
	close(rdPipe[WRITE]);
	for (count = 0; count < 2; count++) {
		char * msj = "9";
		write(wrPipe[WRITE], msj, 1);
		sleep(getpid() % 4); // random sleep...
		if (read(rdPipe[READ], buf, 1) > 0) {
			printf("Message from airline -- %c\n", buf[0]);
		}
	}
	write(wrPipe[WRITE], "0", 1);
	exit(0);	
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
	return TRUE;
	int i;

	for (i = 0; i < city->needsCount; i++) {
		//Item * item = &(city->needs[i]);
		//TODO: verificar cantidad de recursos
	}
}


