#ifndef _AIRLINE_
#define _AIRLINE_

#include "common.h"
#include "plane.h"
#include "ipc.h"

typedef struct {
	int id;
	Map* map;
	int* targetedCities;
	int targetedCitiesSize;
	pthread_t* planesThreads;
	Plane* planes;
	int planesSize;
} Airline;

Airline* createAirline(long id, Map* map, int numberOfPlanes);

void airlineStart(Airline* airline);

#endif

