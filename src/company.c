#include "company.h"

Company *newCompany(char* name, int maxPlaneCount) {
	Company* company = malloc(sizeof(Company));
	company->name = name;
	company->planeCount = 0;
	for (int i = 0; i < maxPlaneCount; ++i) {
		addPlane(company, newPlane(i));
	}
	return company;
}

void addPlane(Company *company, Plane* plane) {
	company->plane[company->planeCount++] = *plane;
}

