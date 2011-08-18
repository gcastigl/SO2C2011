#ifndef _AIRLINE_
#define _AIRLINE_

#include "common.h"
#include "plane.h"
#include "ipc.h"

typedef struct {
	int id;
	int planeCount;
	int* targetedCities;
	int targetedCitiesSize;
} Airline;

Airline* createAirline(long id, int numberOfPlanes);

void airlineStart(Airline* airline);

#endif

