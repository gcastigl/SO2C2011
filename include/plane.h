#ifndef PLANE_H_
#define PLANE_H_

#include "common.h"
#include "map.h"
#include "mathUtil.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>

#define NO_TARGET	-1

Plane *newPlane(int id);

void *planeStart(void* param);

#endif

