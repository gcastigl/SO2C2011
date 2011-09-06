#include "controller/planeLogic.h"

void updateState(Plane* plane);

void* planeStart(void* param) {
    log_debug("Plane started\n");
	Plane* plane = (Plane*) param;
	int turnSemId = semaphore_get(PLANE_COMPANY_ID(plane->id));
	if (turnSemId < 0) {
		fatal("Error initializing semaphore");
	}
	while (1) {
		log_debug("[Plane %d] plays new turn\n", plane->id);
		semaphore_decrement(turnSemId, PLANE_INDEX(plane->id) + 1);
		updateState(plane);
		sleep(1);
		semaphore_increment(turnSemId, 0);
	}
	exit(0);
}

void updateState(Plane* plane) {
	plane->distanceLeft--;
	if (plane->distanceLeft <= 0) {
		plane->cityIdFrom = plane->cityIdTo;
		plane->cityIdTo = NO_TARGET;
	}
	log_debug("[Plane %d] Distance left: %d\n", plane->id, plane->distanceLeft);
}


