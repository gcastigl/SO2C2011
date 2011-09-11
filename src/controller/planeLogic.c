#include "controller/planeLogic.h"

void updateState(Plane* plane);

void* planeStart(void* param) {
	Plane* plane = (Plane*) param;
	int companySemId = semaphore_get(PLANE_COMPANY_ID(plane->id) + 1);
	if (companySemId < 0) {
		fatal("PlaneLogic - Error initializing semaphore");
	}
	semaphore_increment(companySemId, 0); // Notify company that this plane is ready.
	while (1) {
		semaphore_decrement(companySemId, PLANE_INDEX(plane->id) + 1);
		updateState(plane);
		semaphore_increment(companySemId, 0);
	}
	pthread_exit(0);
}

void updateState(Plane* plane) {
	if (plane->distanceLeft > 0) {
		plane->distanceLeft--;
	}
	if (plane->distanceLeft == 0 && plane->cityIdTo != NO_TARGET) {
		plane->cityIdFrom = plane->cityIdTo;
		plane->cityIdTo = NO_TARGET;
	}
}


