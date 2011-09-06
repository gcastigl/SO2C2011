#include "plane.h"

Plane *newPlane(int id, int cityIdFrom) {
	Plane* plane = malloc(sizeof(Plane));
	plane->id = id;
	plane->cityIdFrom = cityIdFrom;
	plane->cityIdTo = NO_TARGET;
	plane->distanceLeft = 0;
	return plane;
}

