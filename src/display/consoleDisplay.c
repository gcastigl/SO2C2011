#include "display/consoleDisplay.h"
/*
void displayCities();
void displayCity(int index);
void displayCompanies();
void displayCompany(Company* company);
void displayPlane(Plane *plane);
void *displaySimulation(void *arg);

void display_start(Map *mapArg) {
	map = mapArg;
	pthread_t display_thread;
	if (!!pthread_create(&display_thread, NULL, displaySimulation, NULL)) {
		fatal("Error creating display thread");
	}
}

void *displaySimulation(void *arg) {
	while(1) {
		printf("\n<--------------------------------------------------\
----------------------------------------------->\n");
		displayCompanies();
		displayCities();
		sleep(1);
	}
	return NULL;
}

void displayCities() {
	printf("********************************************\n");
	for(int i = 0; i < map->cityCount; i++) {
		displayCity(i);
	}
}

void displayCity(int index) {
	printf("City %d ", index);
	City* city = map->city[index];
	for(int i = 0; i < map->cityCount; i++) {
		if (i != index) {
			printf("%2d => %2d    ", i , city->cityDistance[i]);
		}
	}
	printf("\n");
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
	if (plane->cityIdTo != -1) {
		printf("\t\tTo: %d\n", plane->cityIdTo);
	} else {
		printf("\t\tTo: \"No Destination\"\n");
	}
	printf("\t\tDistance left: %d\n", plane->distanceToDestination);
	printf("\t\t-------------------------------\n");
}*/
