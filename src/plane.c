#include "plane.h"

Plane *newPlane(int id, int companyId, int cityIdFrom) {
	Plane* plane = malloc(sizeof(Plane));
	plane->cityIdTo = NO_TARGET;
	plane->distanceToDestination = 0;
	plane->cityIdFrom = cityIdFrom;
	plane->thread = -1;
	plane->id = id;
	plane->ownerCompanyId = companyId;
	return plane;
}

void diffStock(Plane *plane, int itemId, int itemStockDiff) {
	plane->itemStock[itemId] += itemStockDiff;
}

