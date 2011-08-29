#include "plane.h"

Plane *newPlane(int id, int cityIdFrom) {
	Plane* plane = malloc(sizeof(Plane));
	plane->cityIdTo = 0;
	plane->distanceToDestination = 0;
	plane->cityIdFrom = cityIdFrom;
	plane->thread = 0;
	plane->id = id;
	return plane;
}

void diffStock(Plane *plane, int itemId, int itemStockDiff) {
	plane->itemStock[itemId] += itemStockDiff;
}

