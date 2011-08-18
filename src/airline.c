#include "../include/airline.h"

void initPlanes(int planes);

Airline* createAirline(long id, int numberOfPlanes) {
	Airline* airline = (Airline*) malloc(sizeof(Airline));
	airline->id = id;
	airline->planeCount = numberOfPlanes;
	airline->targetedCities = malloc(sizeof(int) * numberOfPlanes);
	int i;
	for (i = 0; i < numberOfPlanes; i++) {
		airline->targetedCities[i] = FALSE;
	}
    ipcSetup(numberOfPlanes);
	initPlanes(numberOfPlanes);
    ipcPostChildSetup(numberOfPlanes);
	return airline;
}

void airlineStart(Airline* airline) {
    int i;
    ipcPackage package;
    package.id = 1;
	while (waitpid(-1, NULL, WNOHANG) != -1) {
        for (i = 0; i < airline->planeCount; i++) {
            if (ipcIsReady(i, WRITE) == TRUE) {
                strcpy(package.data, "Message to plane :D\n");
                sendData(i, package);
            }
        }
        sleep(1);
	}
    return;
}

void initPlanes(int planes) {
	int i;
	for (i = 0; i < planes; i++) {
		switch (fork()) {
			case -1:
				fatal("Fork call error");
                break;
			case 0:
                ipcSetupChild(i);
				planeStart(createPlane(i));
                break;
		}
	}
}


