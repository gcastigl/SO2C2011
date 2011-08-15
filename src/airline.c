#include "../include/airline.h"

void initPlanes(int planes, int** rdPipes, int** wrPipes);
void closeUnusedFds(Airline* airline, int **fds, int pipe);

Airline* createAirline(long id, int numberOfPlanes) {
	Airline* airline = (Airline*) malloc(sizeof(Airline));
	airline->id = id;
	airline->planeCount = numberOfPlanes;
	airline->targetedCities = malloc(sizeof(int) * numberOfPlanes);
	int i;
	for (i = 0; i < numberOfPlanes; i++) {
		airline->targetedCities[i] = FALSE;
	}
	return airline;
}

void airlineProcess(Airline* airline) {
	int i;
	fd_set masterRdFd, masterWrFd, readCpy;
	ipcPackage *data = malloc(sizeof(ipcPackage));
	int** rdPipes = createIntMatrix(airline->planeCount, 2);
	int** wrPipes = createIntMatrix(airline->planeCount, 2);
	
	initPlanes(airline->planeCount, rdPipes, wrPipes);
	// closes all unwanted write file descriptors
	closeUnusedFds(airline, rdPipes, WRITE);
	closeUnusedFds(airline, wrPipes, READ);
	// Sets all the bit masks for the select system call
	FD_ZERO(&masterRdFd);
	FD_SET(0, &masterRdFd);
	for (i = 0; i < airline->planeCount; i++) {
		FD_SET(rdPipes[i][READ], &masterRdFd);
		FD_SET(wrPipes[i][WRITE], &masterWrFd);
	}
	// Call to select with no timeout, it will block until an event occurs
	while (readCpy = masterRdFd, select(rdPipes[airline->planeCount - 1][READ] + 1, &readCpy, NULL, NULL, NULL) > 0) {
		for (i = 0; i < airline->planeCount; i++) {
			if (FD_ISSET(rdPipes[i][READ], &readCpy)) {
				if (read(rdPipes[i][READ], data, PACKAGE_SIZE) > 0) {
					printf("Message from child %d -- %s\n", i, data->message);
					strcpy(data->message, "Response from Airline\n");
					write(wrPipes[i][WRITE], data, PACKAGE_SIZE);
				}
			}
		}
		// if all sub-processes are dead, return to the main program.
		if (waitpid(-1, NULL, WNOHANG) == -1) {
			return;
		}
	}
}

void closeUnusedFds(Airline* airline, int **fds, int pipe) {
	int i;
	for (i = 0; i < airline->planeCount; i++)
		close(fds[i][pipe]);
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


