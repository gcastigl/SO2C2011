#include "../include/airline.h"

#define	MSJ_SIZE	20		// FIXME: To be removed...

void initPlanes(int planes, int** rdPipes, int** wrPipes);

Airline* createAirline(char* name, int numberOfPlanes) {
	Airline* airline = (Airline*) malloc(sizeof(Airline));
	airline->name = name;
	airline->planesCount = numberOfPlanes;
	airline->targetedCities = malloc(sizeof(int) * numberOfPlanes);
	int i;
	for (i = 0; i < numberOfPlanes; i++) {
		airline->targetedCities[i] = FALSE;
	}
	return airline;
}

void airlineProcess(Airline* airline) {
	int** rdPipes = createIntMatrix(airline->planesCount, 2);
	int** wrPipes = createIntMatrix(airline->planesCount, 2);
	initPlanes(airline->planesCount, rdPipes, wrPipes);
	char ch, buf[MSJ_SIZE];
	fd_set masterFD, setFD;
	int i;
	// closes all unwanted write file descriptors
	for (i = 0; i < airline->planesCount; i++) {
		close(rdPipes[i][WRITE]);
		close(wrPipes[i][READ]);
	}
	// Sets all the bit masks for the select system call
	FD_ZERO(&masterFD);
	FD_SET(0, &masterFD);
	for (i = 0; i < airline->planesCount; i++) {
		FD_SET(rdPipes[i][READ], &masterFD);
	}
	// Call to select with no timeout, it will block until an event occurs
	while (setFD = masterFD, select(rdPipes[airline->planesCount - 1][READ] + 1, &setFD, NULL, NULL, NULL) > 0) {
		for (i = 0; i < airline->planesCount; i++) {
			if (FD_ISSET(rdPipes[i][READ], &setFD)) {
				if (read(rdPipes[i][READ], buf, MSJ_SIZE) > 0) {
					printf("Message from child %d -- %s\n", i, buf);
				}
			}
		}
		// if all sub-processes are dead, return to the main program.
		if (waitpid(-1, NULL, WNOHANG) == -1) {
			return;
		}
	}
}

void initPlanes(int planes, int** rdPipes, int** wrPipes) {
	int i;
	for (i = 0; i < planes; i++) {
		if (pipe(rdPipes[i]) == -1 || pipe(wrPipes[i]) == -1) {
			fatal("Pipe call error");
		}
		switch (fork()) {
			case -1:
				fatal("Fork call error");
			case 0:
				planeProcess(createPlane(i), rdPipes[i], wrPipes[i]);
		}
	}
}



