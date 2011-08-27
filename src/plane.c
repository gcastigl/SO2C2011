#include "../include/plane.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>

void updateState(Plane* plane);
void setNewTarget(Plane* plane);
int canSupplyCity(Plane* plane, City* city);
int getScore(Plane* plane, int originCityIndex, City* destination);

Plane* createPlane(Map* map, int id, int initialCityIndex, Item* supplies, int suppliesSize) {
	Plane* plane = (Plane*) malloc(sizeof(Plane));
	plane->id = id;
	plane->map = map;
	plane->distanceToDestination = 0;
	plane->originCityIndex = initialCityIndex;
	plane->destinationCityIndex = -1;
	plane->supplies = supplies;
	plane->suppliesSize = suppliesSize;
	int i;
	printf("------------ Avion %d ------------\n", id);
	for (i = 0; i < suppliesSize; i++) {
		printf("Supply %d -> id: %d - amt: %d\n", i, supplies[i].id, supplies[i].amount);
	}
	printf("-------------------------------------\n");
	return plane;
}

void* planeStart(void* param) {
	Plane* me = (Plane*) param;
	int j=0;
	int semId = semaphore_create(33, 1, 0);	// FIXME: can this 1 cause any problems?
	if (semId == -1) {
		fatal("Error getting semaphore");
	}
	while (1) {
		semaphore_decrement(semId, me->id);
		printf("Hijo %d -> %d\n", me->id, j);
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
	Map* map = plane->map;
	City newCity;
	for (i = 0; i < map->citiesSize; i++) {
		if (i == plane->originCityIndex) {
			// Skip current city...
			continue;
		}
		newCity = map->cities[i];
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
	plane->distanceToDestination = plane->map->distances[plane->originCityIndex][bestCityindex]; 	
}

int canSupplyCity(Plane* plane, City* city) {
	int i, j;
	int itemIdFound;
	for (i = 0; i < plane->suppliesSize; i++) {
		Item planeSupply = plane->supplies[i];
		itemIdFound = FALSE;
		for (j = 0; j < city->needsSize && !itemIdFound; j++) {
			Item cityNeed = city->needs[j];
			if (planeSupply.id == cityNeed.id) {
				itemIdFound = TRUE;	// Stop iterations when item id was found
				if (planeSupply.amount > 0) {
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

int getScore(Plane* plane, int originCityIndex, City* destination) {
	int i, j, score = 0;
	int itemIdFound;
	for (i = 0; i < plane->suppliesSize; i++) {
		Item planeSupply = plane->supplies[i];
		itemIdFound = FALSE;
		for (j = 0; j < destination->needsSize && !itemIdFound; j++) {
			Item cityNeed = destination->needs[j];
			if (planeSupply.id == cityNeed.id) {
				itemIdFound = TRUE;
				score += min(planeSupply.amount, cityNeed.amount);
			}
		}
	}
	return score;
}

