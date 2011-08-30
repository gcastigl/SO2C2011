#include "controller/planeLogic.h"

void readMessages(Plane* plane, int ipcId);

void writeMessages(Plane* plane, int ipcId);

void updateState(Plane* plane);

void setNewTarget(Plane* plane);

int canSupplyCity(Plane* plane, City* city);

int getScore(Plane* plane, int originCityIndex, City* destination);

void* planeStart(void* param) {
	Plane* me = (Plane*) param;
	int turn = 0;
	int ipcId = ipc_get(IPC_KEY);
	int planesTurnSemId = semaphore_get(SEM_PLANE_KEY);
	int companyTurnSemId = semaphore_get(SEM_COMPANY_KEY);
	if (planesTurnSemId < 0 || companyTurnSemId < 0 || ipcId < 0) {
		fatal("Error nitializing initial varibles.");
	}
	while (1) {
		semaphore_decrement(planesTurnSemId, me->id);
		readMessages(me, ipcId);
		//updateState(me);
		printf("------------Hijo %d -> turn: %d-----------------\n", me->id, turn++);
		printf("Distance to destination: %d\n", me->distanceToDestination);
		printf("----------------------------------------------\n\n");
		writeMessages(me, ipcId);
		sleep(1);
		semaphore_increment(companyTurnSemId, 0);
	}
	exit(0);
	return NULL;
	/*
	while (1) {
		pthread_mutex_lock(&(fake_ipc->mutexCounter));
		...
		pthread_mutex_unlock(&(fake_ipc->mutexCounter));
	}*/
}

void readMessages(Plane* plane, int ipcId) {
	IpcPackage* msg = ipc_read(ipcId, plane->ownerCompanyId);
	if (msg != NULL) {
		printf("Plane %ld recieved a msg from its company: %s\n", plane->thread, msg->data);
	}
}

// FIXME: the message instance could be the same every time instaed of creating a new one on every call!!
void writeMessages(Plane* plane, int ipcId) {
	IpcPackage* msg = malloc(sizeof(IpcPackage));
	msg->addressee = plane->ownerCompanyId;
	msg->sender = plane->thread;
	strcpy(msg->data, "This is a message to my company! <(0_0)>\n");
	printf("writing return %d\n", ipc_write(ipcId, msg));
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
		newCity = *map->city[i];
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
	plane->distanceToDestination = map->city[plane->cityIdFrom]->cityDistance[bestCityindex];
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

