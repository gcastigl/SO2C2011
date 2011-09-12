#include "controller/planeLogic.h"

void updateState(Plane* plane);

void* planeStart(void* param) {
	Plane* plane = (Plane*) param;
    char semName[10];
    char semCompName[10];
    sprintf(semName, "c%d_p%d", PLANE_COMPANY_ID(plane->id), PLANE_INDEX(plane->id));
    sprintf(semCompName, "c%d", PLANE_COMPANY_ID(plane->id));
    S_POST(semName); // Notify company that this plane is ready.
	while (1) {
        S_WAIT(semName);
		updateState(plane);
        S_POST(semCompName);
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


