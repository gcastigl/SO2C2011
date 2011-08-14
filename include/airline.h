#ifndef _AIRLINE_
#define _AIRLINE_

#include "common.h"
#include "plane.h"
#include <sys/types.h>
#include <string.h>
#include <unistd.h>


typedef struct {
	int id;
	int planeCount;
	int* targetedCities;
} Airline;

Airline* createAirline(long id, int numberOfPlanes);

void airlineProcess(Airline* airline);

#endif
