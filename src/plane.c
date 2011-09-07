#include "plane.h"

Plane *newPlane(int id, int cityIdFrom, int itemCount) {
	Plane* plane = malloc(sizeof(Plane));
	plane->id = id;
	plane->cityIdFrom = cityIdFrom;
	plane->cityIdTo = NO_TARGET;
	plane->distanceLeft = 0;
	plane->itemCount = itemCount;
	plane->itemStock = malloc(sizeof(int) * itemCount);
	return plane;
}

