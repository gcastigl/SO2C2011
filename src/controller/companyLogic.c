#include "controller/companyLogic.h"

int initializeCompany();
void wakeUpPlanes(int semId);
void waitUntilPlanesReady(int semId);
void updateDestinations();
void updateMapItems(Map* map, Plane* plane);
void setNewTarget(Map* map, Plane* plane);
int getScore(Plane* plane, int cityId);

static Company *company;
static Map *map;
static pthread_t* planeThreadId;

/*
 * 1 - Initialize company.
 * 2 - Update map.
 * 3 - Wake up planes.
 * 4 - Update planes.
 * 5 - Send map updates.
 */
void companyStart(Map* initialMap, Company* cmp) {
    printf("Creating company...\n");
	company = cmp;
	map = initialMap;
	int turnsSemId = initializeCompany();
	for (int i = 0; i < 5; i++) {
		log_debug("[Company %d] Playing one turn\n", company->id);
		wakeUpPlanes(turnsSemId);
		waitUntilPlanesReady(turnsSemId);
		updateDestinations();
		log_debug("[Company %d] Finished turn OK\n", company->id);
	}
    exit(0);
}

int initializeCompany() {
	planeThreadId = malloc(sizeof(pthread_t) * company->planeCount);
	int turnsSemId = semaphore_create(company->id, (company->planeCount) + 1, FLAGS);
	if (turnsSemId < 0) {
		fatal("Company - Error initializing semaphore.");
	}
	for(int i = 0; i < company->planeCount; i++) {
		pthread_create(planeThreadId + i, NULL, planeStart, company->plane[i]);
	}
	return turnsSemId;
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
		if (company->plane[i]->distanceLeft == 0) {
			updateMapItems(map, company->plane[i]);
			setNewTarget(map, company->plane[i]);
		}
	}
}

void updateMapItems(Map* map, Plane* plane) {
	log_debug("[Company %d] Updating items for plane %d\n", company->id, plane->id);
	for (int i = 0; i < plane->itemCount; ++i) {
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

	for (i = 0; i < map->cityCount; i++) {
		int routeLength = map->cityDistance[plane->cityIdFrom][i];
		if (i == plane->cityIdFrom || routeLength == 0) {
			// Skip if current city or route does not exists
			continue;
		}
		newTargetScore = getScore(plane, i);
		if (bestCityScore < newTargetScore) {
			bestCityScore = newTargetScore;
			bestCityindex = i;
		}
	}
	if (bestCityindex == NO_TARGET) {
		// No more cities can be supplied
		log_debug("[Company %d] No more cities can be supplied by %d\n", company->id, plane->id);
		pthread_kill(planeThreadId + PLANE_INDEX(plane->id), SIGKILL);
		planeThreadId[PLANE_INDEX(plane->id)] = -1;
		return;
	}
	// Set new distance from currentTargetId to newTaget
	plane->cityIdTo = bestCityindex;
	plane->distanceLeft = map->cityDistance[plane->cityIdFrom][bestCityindex];
}

int getScore(Plane* plane, int cityId) {
	int score = 0;
	for (int i = 0; i < plane->itemCount; i++) {
		if (plane->itemStock[i] > 0 && map->city[cityId]->itemStock[i] < 0) {
			score += min(plane->itemStock[i], -map->city[cityId]->itemStock[i]);
		}
	}
	return score;
}

