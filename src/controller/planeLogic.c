#include "controller/planeLogic.h"

void updateState(Plane* plane);

void setNewTarget(Plane* plane);

int canSupplyCity(Plane* plane, City* city);

int getScore(Plane* plane, int originCityIndex, City* destination);

void* planeStart(void* param) {
	Plane* me = (Plane*) param;
	int turn = 0;
	int planesTurnSemId = semaphore_create(SEM_PLANE_KEY, 1, 0600);
	int companyTurnSemId = semaphore_create(SEM_COMPANY_KEY, 1, 0600);
	if (planesTurnSemId <= 0 || companyTurnSemId <= 0) {
		fatal("Error creating semaphore");
	}
	while (1) {
		semaphore_decrement(planesTurnSemId, me->id);
		printf("------------Hijo %d -> turn: %d-----------------\n", me->id, turn++);
		printf("Distance to destination: %d\n", me->distanceToDestination);
		printf("----------------------------------------------\n\n");
		//updateState(me);
		sleep(1);
		semaphore_increment(companyTurnSemId, 0);
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
		plane->cityIdFrom = plane->cityIdTo;
		plane->cityIdTo = NO_TARGET;
		setNewTarget(plane);
	}
}

void setNewTarget(Plane* plane) {
	int i, bestCityScore = -1, bestCityindex = NO_TARGET, newTargetScore;
	City newCity;
	for (i = 0; i < map->cityCount; i++) {
		if (i == plane->cityIdFrom) {
			// Skip current city...
			continue;
		}
		newCity = map->city[i];
		if (canSupplyCity(plane, &newCity)) {
			newTargetScore = getScore(plane, plane->cityIdFrom, &newCity);
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
	plane->cityIdTo = bestCityindex;
	plane->distanceToDestination = map->city[plane->cityIdFrom].cityDistance[bestCityindex];
}

int canSupplyCity(Plane* plane, City* city) {
	for (int i = 0; i < map->itemCount; i++) {
		if (plane->itemStock[i] > 0 && city->itemStock[i] < 0) {
			return TRUE;
		}
	}
	return FALSE;
}

int getScore(Plane* plane, int originCityIndex, City* destination) {
	int score = 0;
	for (int i = 0; i < map->itemCount; i++) {
		if (plane->itemStock[i] > 0 && destination->itemStock[i] < 0) {
			score += min(plane->itemStock[i], -destination->itemStock[i]);
		}
	}
	return score;
}
