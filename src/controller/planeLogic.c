#include "controller/planeLogic.h"

void updateState(Plane* plane);

void* planeStart(void* param) {
    log_debug("Plane started");
	Plane* plane = (Plane*) param;
	int companySemId = semaphore_get(PLANE_COMPANY_ID(plane->id));
	printf("plane has sem: %d\n", companySemId);
	if (companySemId < 0) {
		fatal("PlaneLogic - Error initializing semaphore");
	}
	while (1) {
		semaphore_decrement(companySemId, PLANE_INDEX(plane->id) + 1);
		log_debug("[Plane %d] plays new turn", plane->id);
		//updateState(plane);
		sleep(10);
		log_debug("[Plane %d] end turn", plane->id);
		semaphore_increment(companySemId, 0);
	}
	exit(0);
}

void updateState(Plane* plane) {
	plane->distanceLeft--;
	if (plane->distanceLeft <= 0) {
		plane->cityIdFrom = plane->cityIdTo;
		plane->cityIdTo = NO_TARGET;
	}
	log_debug("[Plane %d] Distance left: %d", plane->id, plane->distanceLeft);
}


