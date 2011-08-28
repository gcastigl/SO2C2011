#include "../include/plane.h"

Plane *newPlane(int id) {
	Plane* plane = malloc(sizeof(Plane));
	plane->destinationCityIndex = 0;
	plane->distanceToDestination = 0;
	plane->originCityIndex = 0;
	plane->thread = 0;
	plane->id = id;
	plane->itemStock = malloc(20 * sizeof(int));  // FIXME: remove this hard coded value
	return plane;
}

void diffStock(Plane *plane, int itemId, int itemStockDiff) {
	plane->itemStock[itemId] += itemStockDiff;
}

