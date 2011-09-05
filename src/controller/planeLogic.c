#include "controller/planeLogic.h"

void readMessages(Plane* plane);
void writeMessages(Plane* plane);
void updateState(Plane* plane);
void setNewTarget(Plane* plane);
int canSupplyCity(Plane* plane, City* city);
int getScore(Plane* plane, int originCityIndex, City* destination);

void* planeStart(void* param) {
    log_debug("Plane started\n");
	Plane* plane = (Plane*) param;
	int turnSemId = semaphore_get(PLANE_COMPANY_ID(plane->id) + 100);
	if (turnSemId < 0) {
		fatal("Error initializing semaphore");
	}
	while (1) {
		semaphore_decrement(turnSemId, PLANE_INDEX(plane->id) + 1);
		// readMessages(plane);
		// updateState(plane);
		writeMessages(plane);
		sleep(1);
		semaphore_increment(turnSemId, 0);
	}
	exit(0);
}

void readMessages(Plane* plane) {
	char msg[DATA_SIZE];
	log_debug("[Plane %d] Reading from company...\n", plane->id);
	int msgRead = ipc_read(plane->id, PLANE_COMPANY_ID(plane->id), msg);
	if (msgRead != -1) {
		log_debug("[Plane %d] Response response from company: %s", plane->id, msg);
	} else {
		log_debug("[Plane %d] NO messages from company\n", plane->id);
		perror("readMessages");
	}
}

void writeMessages(Plane* plane) {
	char msg[DATA_SIZE];
	sprintf(msg, "plane %d needs some information\n", plane->id);
	log_debug("[Plane %d] Writing to company...\n", plane->id);
	int writeReturn = ipc_write(plane->id, PLANE_COMPANY_ID(plane->id), msg);
	if (writeReturn != -1) {
		log_debug("[Message sent OK]\n");
	} else {
		log_debug("[Plane %d] ERROR writing to company...\n", plane->id);
	}
}

void updateState(Plane* plane) {
	plane->distanceToDestination--;
	log_debug("[Plane %d] Distance left: %d\n", plane->id, plane->distanceToDestination);
	if (plane->distanceToDestination <= 0) {
		plane->cityIdFrom = plane->cityIdTo;
		plane->cityIdTo = NO_TARGET;
		// setNewTarget(plane);
	}
}

void setNewTarget(Plane* plane) {
	int i, bestCityScore = -1, bestCityindex = NO_TARGET, newTargetScore;
	City* newCity;
	for (i = 0; i < map->cityCount; i++) {
		int routeLength = map->city[plane->cityIdFrom]->cityDistance[i];
		if (i == plane->cityIdFrom || routeLength <= 0) {
			// Skip if current city or route does not exists
			continue;
		}
		newCity = map->city[i];
		printf("Pasando por la ciduad...\n");
		if (canSupplyCity(plane, newCity)) {
			newTargetScore = getScore(plane, plane->cityIdFrom, newCity);
			if (bestCityScore < newTargetScore) {
				bestCityScore = newTargetScore;
				bestCityindex = i;
			}
		}
	}

	if (bestCityindex == NO_TARGET) {
		// No more cities can be supplied
		//pthread_exit(NULL);
		printf("Me muero\n\n");
		return;
	}
	// Set new distance from currentTargetId to newTaget
	plane->cityIdTo = bestCityindex;
	//plane->distanceToDestination = map->city[plane->cityIdFrom]->cityDistance[bestCityindex];
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

