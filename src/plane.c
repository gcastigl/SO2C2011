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
			if (newTargetScore == -1 && bestCityScore < newTargetScore) {
				bestCityScore = newTargetScore;
				bestCityindex = i;
			}
		}
	}
	if (bestCityindex != NO_TARGET) {
		// Set new distance from currentTargetId to newTaget
		plane->distanceToDestination = plane->map->distances[plane->originCityIndex][bestCityindex]; 	
		plane->destinationCityIndex = bestCityindex;
	}
	// TODO: else exit(0) ?
}

int canSupplyCity(Plane* plane, City* city) {
	int i, j;
	for (i = 0; i < plane->suppliesSize; i++) {
		Item planeSupply = plane->supplies[i];
		for (j = 0; j < city->needsSize; j++) {
			Item cityNeed = city->needs[j];
			if (planeSupply.id == cityNeed.id && planeSupply.amount > 0) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

int getScore(Plane* plane, int originCityIndex, City* destination) {
	int i, j, score = 0;
	for (i = 0; i < plane->suppliesSize; i++) {
		Item planeSupply = plane->supplies[i];
		for (j = 0; j < destination->needsSize; j++) {
			Item cityNeed = destination->needs[j];
			if (planeSupply.id == cityNeed.id ) {
				score += min(planeSupply.amount, cityNeed.amount);
			}
		}
	}
	return score;
}

