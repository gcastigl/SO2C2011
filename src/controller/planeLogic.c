#include "controller/planeLogic.h"

void readMessages(Plane* plane);
void writeMessages(Plane* plane);
void updateState(Plane* plane);

void* planeStart(void* param) {
    log_debug("Plane started\n");
	Plane* plane = (Plane*) param;
	int turnSemId = semaphore_get(PLANE_COMPANY_ID(plane->id));
	if (turnSemId < 0) {
		fatal("Error initializing semaphore");
	}
	while (1) {
		semaphore_decrement(turnSemId, PLANE_INDEX(plane->id) + 1);
		// readMessages(plane);
		updateState(plane);
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
		perror("");
		log_debug("[Plane %d] ERROR writing to company...\n", plane->id);
	}
}

void updateState(Plane* plane) {
	plane->distanceToDestination--;
	if (plane->distanceToDestination <= 0) {
		plane->cityIdFrom = plane->cityIdTo;
		plane->cityIdTo = NO_TARGET;
	}
	log_debug("[Plane %d] Distance left: %d\n", plane->id, plane->distanceToDestination);
}


