#ifndef _AIRLINE_
#define _AIRLINE_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"

typedef struct {
	char* name;
	int planesCount;
	int* targetedCities;
} Airline;

Airline* createAirline(char* name, int numberOfPlanes);
void airlineProcess(Airline* airline);

#endif
