#ifndef _AIRLINE_
#define _AIRLINE_

#include "common.h"
#include <string.h>

typedef struct {
	long id;
	int planeCount;
	int* targetedCities;
} Airline;

Airline* createAirline(long id, int numberOfPlanes);

void airlineProcess(Airline* airline);

#endif
