#ifndef _PLANE_
#define _PLANE_

#include "common.h"
#include <string.h>

typedef struct {
	long id;
	int targetId;
	int distanceLeft;
	Item* supplies;
} Plane;

Plane* createPlane(int id);

void planeProcess(Plane* plane, int* wrPipe, int** rdPipe);

#endif
