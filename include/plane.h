#ifndef _PLANE_
#define _PLANE_

#include "common.h"
#include "map.h"

#define NO_TARGET	-1

typedef struct {
	int id;
	int targetId;
	int distanceLeft;
	Item* supplies;
} Plane;

Plane* createPlane(int id);

void planeProcess(Plane* plane, int* wrPipe, int* rdPipe);

#endif
