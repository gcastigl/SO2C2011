#include "display/consoleDisplay.h"

void displayCompanies();

void displayCompany(Company* company);

void displayPlane(Plane *plane);

void displaySimulation() {
	initChildSignalHandler();
	while(1) {
		printf("\n-----------------------------------------------\n");
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
	printf("\n----- Company %d - %20s ---------\n", company->id, company->name);
	for(int i = 0; i < company->planeCount; i++) {
		displayPlane(company->plane[i]);
	}
	printf("\n");
}


void displayPlane(Plane *plane) {
	printf("\n\t------- Plane %d ----------\n", plane->id);
	printf("\tFrom: %d\n", plane->cityIdFrom);
	printf("\tTo: %d\n", plane->cityIdTo);
	printf("\tDistance left: %d\n", plane->distanceToDestination);
	printf("\n");
}
