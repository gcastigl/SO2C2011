#include "company.h"

Company *newCompany(int id, char* name, int maxPlaneCount) {
	Company* company = malloc(sizeof(Company));
	company->id = id;
	company->planeCount = maxPlaneCount;
	company->plane = malloc(sizeof(Plane) * maxPlaneCount);
	strcpy(company->name, name);
	return company;
}

void company_setPlane(Company *company, Plane *plane, int index) {
	if (0 < index && index < company->planeCount) {
		company->plane[index] = plane;
	} else {
		log_error("Trying to append plane to invalid index, %d\n", index);
	}
}

