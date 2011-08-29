#include "company.h"

Company *newCompany(char* name, int maxPlaneCount) {
	Company* company = malloc(sizeof(Company));
	mstrcpy(&(company->name), name);
	company->planeCount = 0;
	return company;
}

void addPlane(Company *company, Plane *plane) {
	company->plane[company->planeCount++] = plane;
}

