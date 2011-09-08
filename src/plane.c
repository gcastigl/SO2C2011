#include "plane.h"

Plane *newPlane(int id, int cityIdFrom, int itemCount) {
	Plane* plane = malloc(sizeof(Plane));
	plane->id = id;
	plane->cityIdFrom = cityIdFrom;
	plane->cityIdTo = NO_TARGET;
	plane->distanceLeft = 0;
	plane->itemCount = itemCount;
	plane->itemStock = malloc(sizeof(int) * itemCount);
	for (int i = 0; i < itemCount; ++i) {
		plane->itemStock[i] = 0;
	}
	return plane;
}

/*
 * After this call, the pointer becames un-usable.
 */
void plane_free(Plane* plane) {
	free(plane->itemStock);
	free(plane);
}
