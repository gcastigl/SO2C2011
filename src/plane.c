#include "../include/plane.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>

Plane *newPlane() {
	Plane* plane = malloc(sizeof(Plane));
	plane->destinationCityIndex = 0;
	plane->distanceToDestination = 0;
	plane->originCityIndex = 0;
	plane->thread = 0;
	plane->itemStock = malloc(20*sizeof(int));
	return plane;
}

void diffStock(Plane *plane, int itemId, int itemStockDiff) {
	plane->itemStock[itemId] += itemStockDiff;
}

void updateState(Plane* plane);
void setNewTarget(Plane* plane);
int canSupplyCity(Plane* plane, City* city);
int getScore(Plane* plane, int originCityIndex, City* destination);

void* planeStart(void* param) {
	Plane* me = (Plane*) param;
	int j=0;
	int semId = semaphore_create(33, 1, 0);	// FIXME: can this 1 cause any problems?
	if (semId == -1) {
		fatal("Error getting semaphore");
	}
	while (1) {
		semaphore_decrement(semId, (int) me->thread);
		printf("Hijo %d -> %d\n", (int) me->thread, j);
		j++;
	}
	exit(0);
	return NULL;
	/*
	FakeIPC* fake_ipc = (FakeIPC*) param;
	int i, elementoDistinto;
	while (1) {
		pthread_mutex_lock(&(fake_ipc->mutexCounter));
		
		elementoDistinto = 0;
		for ( i = 1; i < fake_ipc->bufferSize; i++) {
			if (fake_ipc->buffer[0] != fake_ipc->buffer[i]) {
				elementoDistinto = 1;
				break;
			}
		}
		if (elementoDistinto)
			printf ("Hijo  : Error. Elementos de buffer distintos\n");
		else
			printf ("Hijo  : Correcto\n");
		
		pthread_mutex_unlock(&(fake_ipc->mutexCounter));
	}

    printf("Plane %d created\nWaiting for data...\n", plane->id);
    ipcPackage data;
    data = getData(plane->id);
    printf("Data for plane %d: %s\n",plane->id, data.data);
	exit(0);*/
}

void updateState(Plane* plane) {
	plane->distanceToDestination--;
	if (plane->distanceToDestination <= 0) {
		plane->originCityIndex = plane->destinationCityIndex;
		plane->destinationCityIndex = NO_TARGET;
		setNewTarget(plane);
	}
}

void setNewTarget(Plane* plane) {
	int i, bestCityScore = -1, bestCityindex = NO_TARGET, newTargetScore;
	City newCity;
	for (i = 0; i < map->cityCount; i++) {
		if (i == plane->originCityIndex) {
			// Skip current city...
			continue;
		}
		newCity = map->city[i];
		if (canSupplyCity(plane, &newCity)) {
			newTargetScore = getScore(plane, plane->originCityIndex, &newCity);
			if (bestCityScore < newTargetScore) {
				bestCityScore = newTargetScore;
				bestCityindex = i;
			}
		}
	}
	
	if (bestCityindex == NO_TARGET) {
		// No more cities can be supplied
		pthread_exit(NULL);
	}
	// Set new distance from currentTargetId to newTaget
	plane->destinationCityIndex = bestCityindex;
	plane->distanceToDestination = map->city[plane->originCityIndex].cityDistance[bestCityindex];
}

int canSupplyCity(Plane* plane, City* city) {
	int i;
	for (i = 0; i < map->itemCount; i++) {
		if (plane->itemStock[i] > 0 && city->itemStock[i] < 0) {
			return TRUE;
		}
	}
	return FALSE;
}

int getScore(Plane* plane, int originCityIndex, City* destination) {
	int i, score = 0;
	for (i = 0; i < map->itemCount; i++) {
		if (plane->itemStock[i] > 0 && destination->itemStock[i] < 0) {
			score += min(plane->itemStock[i], -destination->itemStock[i]);
		}
	}
	return score;
}

