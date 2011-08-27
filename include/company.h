#ifndef _AIRLINE_
#define _AIRLINE_

#include "common.h"
#include "plane.h"
#include "ipc.h"

void companyStart(Company *company);
Company *newCompany(char* name, int maxPlaneCount);

#endif

