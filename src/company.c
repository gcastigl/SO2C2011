#include "company.h"

Company *newCompany(int id, char* name, int maxPlaneCount) {
	Company* company = malloc(sizeof(Company));
	company->id = id;
	mstrcpy(&(company->name), name);
	company->planeCount = 0;
	return company;
}

void addPlane(Company *company, Plane *plane) {
	company->plane[company->planeCount++] = plane;
}

