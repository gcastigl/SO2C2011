#include "controller/companyLogic.h"

void wakeUpPlanes(int semId);
void waitUntilPlanesReady(int semId);

void updateDestinations();
void updateMapItems(Map* map, Plane* plane);
void setNewTarget(Map* map, Plane* plane);
int getScore(Map* map, int cityId, Plane* plane);

static Company* company;

/*
 * 1 - Wake up planes.
 * 2 - Read & process plane messages.
 */
void companyStart(Company* cmp) {
    printf("Creating company...\n");
    company = cmp;
	int turnsSemId = semaphore_create(company->id, (company->planeCount) + 1, FLAGS);
	if (turnsSemId < 0) {
		fatal("Company - Error initializing semaphore.");
	}
	for(int i = 0; i < company->planeCount; i++) {
		pthread_create(&(company->plane[i]->thread), NULL, planeStart, company->plane[i]);
	}
	for (int i = 0; i < 5; i++) {
		wakeUpPlanes(turnsSemId);
		waitUntilPlanesReady(turnsSemId);
		updateDestinations();
	}
    exit(0);
}

void wakeUpPlanes(int semId) {
	printf("Planes wake up!\n");
	log_debug("Planes wake up!\n");
	for(int i = 0; i < company->planeCount; i++) {
		semaphore_increment(semId, PLANE_INDEX(company->plane[i]->id) + 1);
	}
}

void waitUntilPlanesReady(int semId) {
	for(int i = 0; i < company->planeCount; i++) {
		semaphore_decrement(semId, 0);
	}
	printf("[Company %d] Waiting done!...\n", company->id);
}

void updateDestinations() {
	Map *map = readMap();
	for(int i = 0; i < company->planeCount; i++) {
		if (company->plane[i]->distanceToDestination == 0) {
			updateMapItems(map, company->plane[i]);
			setNewTarget(map, company->plane[i]);
		}
	}
}

void updateMapItems(Map* map, Plane* plane) {
	for (int i = 0; i < map->itemCount; ++i) {
		int cityStock = map->city[plane->cityIdFrom]->itemStock[i];
		int planeStock = plane->itemStock[i];
		if ( cityStock < 0 && planeStock > 0) {
			int supplies = min(-cityStock, planeStock);
			plane->itemStock[i] -= supplies;
			map->city[plane->cityIdFrom]->itemStock[i] += supplies;
		}
	}
}

void setNewTarget(Map* map, Plane* plane) {
	int i;
	int newTargetScore;
	int bestCityScore = 0;
	int bestCityindex = NO_TARGET;
	City* newCity;

	for (i = 0; i < map->cityCount; i++) {
		int routeLength = map->city[plane->cityIdFrom]->cityDistance[i];
		if (i == plane->cityIdFrom) {
			// Skip if current city or route does not exists
			continue;
		}
		newCity = map->city[i];
		printf("Pasando por la ciduad...\n");
		newTargetScore = getScore(plane, plane->cityIdFrom, newCity);
		if (bestCityScore < newTargetScore) {
			bestCityScore = newTargetScore;
			bestCityindex = i;
		}
	}
	if (bestCityindex == NO_TARGET) {
		// No more cities can be supplied
		log_debug("Avion %d tiene q morir\n", plane->id);
		//TODO: KILL THIS MOTHERFUCKER!!
		return;
	}
	// Set new distance from currentTargetId to newTaget
	plane->cityIdTo = bestCityindex;
	plane->distanceToDestination = map->city[plane->cityIdFrom]->cityDistance[bestCityindex];
}

int getScore(Map* map, int cityId, Plane* plane) {
	int score = 0;
	for (int i = 0; i < map->itemCount; i++) {
		if (plane->itemStock[i] > 0 && map->city[cityId]->itemStock[i] < 0) {
			score += min(plane->itemStock[i], -map->city[cityId]->itemStock[i]);
		}
	}
	return score;
}

