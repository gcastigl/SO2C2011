#include "display/consoleDisplay.h"

void displayCompanies();

void displayCompany(Company* company);

void displayPlane(Plane *plane);

void displaySimulation() {
	while(1) {
		printf("\n<--------------------------------------------------\
----------------------------------------------->\n");
		displayCompanies();
		sleep(1);
	}
}

void displayCompanies() {
	for(int i = 0; i < map->companyCount; i++) {
		displayCompany(map->company[i]);
	}
}

void displayCompany(Company *company) {
	printf("\t----- Company %d - %20s ---------\n", company->id, company->name);
	for(int i = 0; i < company->planeCount; i++) {
		displayPlane(company->plane[i]);
	}
	printf("\t-------------------------------------------------------------\n");
}


void displayPlane(Plane *plane) {
	printf("\t\t------- Plane %d ----------\n", plane->id);
	printf("\t\tFrom: %d\n", plane->cityIdFrom);
	printf("\t\tTo: %d\n", plane->cityIdTo);
	printf("\t\tDistance left: %d\n", plane->distanceToDestination);
	printf("\t\t-------------------------------\n");
}
