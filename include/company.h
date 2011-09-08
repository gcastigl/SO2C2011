#ifndef _AIRLINE_
#define _AIRLINE_

#include "common.h"
#include "plane.h"

Company *newCompany(int id, char* name, int maxPlaneCount);

void company_setPlane(Company *company, Plane *plane, int index);

void company_free(Company* company, int freePlanes);

#endif

