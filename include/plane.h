#ifndef _PLANE_
#define _PLANE_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"

typedef struct {
	int targetId;
	int distanceLeft;
	int id;
	Item* supplies;
} Plane;

Plane* createPlane(int id);
void planeProcess(Plane* plane, int* pipe);

#endif
