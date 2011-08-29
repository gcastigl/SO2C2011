#ifndef _AIRLINE_
#define _AIRLINE_

#include "common.h"
#include "plane.h"
#include "ipc.h"
#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

Company *newCompany(char* name, int maxPlaneCount);

void addPlane(Company *company, Plane *plane);

void companyStart(Company *company);

#endif

