#include "../include/plane.h"

void updateState(Plane* plane);
void setNewTarget(Plane* plane);
int canSupplyCity(Plane* plane, City* city);
int getScore(Plane* plane, int originCityIndex, City* destination);

Plane* createPlane(Map* map, int id, int initialCityIndex, Item* supplies, int suppliesSize) {
	Plane* plane = (Plane*) malloc(sizeof(Plane));
	plane->id = id;
	plane->map = map;
	plane->distanceToDestination = 0;
	plane->originCityIndex = initialCityIndex;
	plane->destinationCityIndex = -1;
	plane->supplies = supplies;
	plane->suppliesSize = suppliesSize;
	return plane;
}

void planeStart(Plane* plane) {
    printf("Plane %d created\nWaiting for data...\n", plane->id);
    ipcPackage data;
    data = getData(plane->id);
    printf("Data for plane %d: %s\n",plane->id, data.data);
	exit(0);
}

void updateState(Plane* plane) {
	plane->distanceToDestination--;
	if (plane->distanceToDestination <= 0) {
		plane->originCityIndex = plane->destinationCityIndex;
		plane->destinationCityIndex = NO_TARGET;
		setNewTarget(plane);
	}
}

void setNewTarget(Plane* plane) {
	int i, bestCityScore = -1, bestCityindex = NO_TARGET, newTargetScore;
	Map* map = plane->map;
	City newCity;
	for (i = 0; i < map->citiesSize; i++) {
		if (i == plane->originCityIndex) {
			// Skip current city...
			continue;
		}
		newCity = map->cities[i];
		if (canSupplyCity(plane, &newCity)) {
			newTargetScore = getScore(plane, plane->originCityIndex, &newCity);
			if (bestCityScore < newTargetScore) {
				bestCityScore = newTargetScore;
				bestCityindex = i;
			}
		}
	}
	
	if (bestCityindex == NO_TARGET) {
		// No more cities can be supplied
		exit(0);
	}
	// Set new distance from currentTargetId to newTaget
	plane->destinationCityIndex = bestCityindex;
	plane->distanceToDestination = plane->map->distances[plane->originCityIndex][bestCityindex]; 	
}

int canSupplyCity(Plane* plane, City* city) {
	int i, j;
	int itemIdFound;
	for (i = 0; i < plane->suppliesSize; i++) {
		Item planeSupply = plane->supplies[i];
		itemIdFound = FALSE;
		for (j = 0; j < city->needsSize && !itemIdFound; j++) {
			Item cityNeed = city->needs[j];
			if (planeSupply.id == cityNeed.id) {
				itemIdFound = TRUE;	// Stop iterations when item id was found
				if (planeSupply.amount > 0) {
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

int getScore(Plane* plane, int originCityIndex, City* destination) {
	int i, j, score = 0;
	int itemIdFound;
	for (i = 0; i < plane->suppliesSize; i++) {
		Item planeSupply = plane->supplies[i];
		itemIdFound = FALSE;
		for (j = 0; j < destination->needsSize && !itemIdFound; j++) {
			Item cityNeed = destination->needs[j];
			if (planeSupply.id == cityNeed.id) {
				itemIdFound = TRUE;
				score += min(planeSupply.amount, cityNeed.amount);
			}
		}
	}
	return score;
}

