#ifndef _PLANE_
#define _PLANE_

#include "common.h"
#include <string.h>

typedef struct {
	int targetId;
	int distanceLeft;
	int id;
	Item* supplies;
} Plane;

Plane* createPlane(int id);

void planeProcess(Plane* plane, int* wrPipe, int** rdPipe);

#endif
