#include "controller/planeLogic.h"

void updateState(Plane* plane);

void* planeStart(void* param) {
	Plane* plane = (Plane*) param;
	int companySemId = semaphore_get(PLANE_COMPANY_ID(plane->id));
	if (companySemId < 0) {
		fatal("PlaneLogic - Error initializing semaphore");
	}
	semaphore_increment(companySemId, 0); // Notify company that this plane is ready.
	while (1) {
		semaphore_decrement(companySemId, PLANE_INDEX(plane->id) + 1);
		log_debug(10, "[Plane %d] plays new turn", plane->id);
		updateState(plane);
		log_debug(10, "[Plane %d] end turn", plane->id);
		semaphore_increment(companySemId, 0);
	}
	exit(0);
}

void updateState(Plane* plane) {
	if (plane->distanceLeft > 0) {
		plane->distanceLeft--;
	}
	if (plane->distanceLeft == 0 && plane->cityIdTo != NO_TARGET) {
		plane->cityIdFrom = plane->cityIdTo;
		plane->cityIdTo = NO_TARGET;
	}
	log_debug(10, "[Plane %d] Distance left: %d", plane->id, plane->distanceLeft);
}


