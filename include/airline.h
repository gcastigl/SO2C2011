#ifndef _AIRLINE_
#define _AIRLINE_

#include "common.h"
#include <string.h>

typedef struct {
	char* name;
	int planesCount;
	int* targetedCities;
} Airline;

Airline* createAirline(char* name, int numberOfPlanes);
void airlineProcess(Airline* airline);

#endif
