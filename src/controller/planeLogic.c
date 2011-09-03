#include "controller/planeLogic.h"

void readMessages(Plane* plane, int ipcId);
void writeMessages(Plane* plane, int ipcId);
void updateState(Plane* plane);
void setNewTarget(Plane* plane);
int canSupplyCity(Plane* plane, City* city);
int getScore(Plane* plane, int originCityIndex, City* destination);

void* planeStart(void* param) {
	Plane* me = (Plane*) param;
	int ipcId = ipc_get(IPC_KEY);
	int planesTurnSemId = semaphore_get(SEM_PLANE_KEY);
	int companyTurnSemId = semaphore_get(SEM_COMPANY_KEY);
	if (planesTurnSemId < 0 || companyTurnSemId < 0 || ipcId < 0) {
		fatal("Error initializing varibles.");
	}
	while (1) {
		semaphore_decrement(planesTurnSemId, me->id - MIN_PLANE_ID);
		readMessages(me, ipcId);
		// updateState(me);
		writeMessages(me, ipcId);
		sleep(1);
		semaphore_increment(companyTurnSemId, 0);
	}
	exit(0);
}

void readMessages(Plane* plane, int ipcId) {
	IpcPackage msg;
	log_debug("[Plane %d] Reading from company...\n", plane->id);
	int msgRead = ipc_read(ipcId, plane->id, &msg);
	if (msgRead != -1) {
		log_debug("[Plane %d] Response response from company: %s", plane->id, msg.data);
	} else {
		log_debug("[Plane %d] NO messages from company\n", plane->id);
		perror("");
	}
}

void writeMessages(Plane* plane, int ipcId) {
	IpcPackage* msg = malloc(sizeof(IpcPackage));
	msg->addressee = plane->ownerCompanyId;
	msg->sender = plane->id;
	sprintf(msg->data, "plane %d needs some information\n", plane->id);
	log_debug("[Plane %d] Writing to company...\n", plane->id);
	int writeReturn = ipc_write(ipcId, msg);
	if (writeReturn != -1) {
		log_debug("[Message sent OK]\n");
	} else {
		log_debug("[Plane %d] ERROR writing to company...\n", plane->id);
	}
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

