#include "../include/airline.h"

#define	MSJ_SIZE	20		// FIXME: To be removed...

int** initPlanes(int planes);

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
	int** pipes = initPlanes(airline->planesCount);
	char ch, buf[MSJ_SIZE];
	fd_set masterFD, setFD;
	int i;
	// closes all unwanted write file descriptors
	for (i = 0; i < airline->planesCount; i++) {
		close(pipes[i][1]);
	}
	// Sets all the bit masks for the select system call
	FD_ZERO(&masterFD);
	FD_SET(0, &masterFD);
	for (i = 0; i < airline->planesCount; i++) {
		FD_SET(pipes[i][0], &masterFD);
	}
	// Call to select with no timeout, it will block until an event occurs
	while (setFD = masterFD, select(pipes[airline->planesCount - 1][0] + 1, &setFD, NULL, NULL, NULL) > 0) {
		for (i = 0; i < airline->planesCount; i++) {
			if (FD_ISSET(pipes[i][0], &setFD)) {
				if (read(pipes[i][0], buf, MSJ_SIZE) > 0) {
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

int** initPlanes(int planes) {
	int i;
	int** pipes = createIntMatrix(planes, 2);
	for (i = 0; i < planes; i++) {
		if (pipe(pipes[i]) == -1) {
			fatal("Pipe call error");
		}
		switch (fork()) {
			case -1:
				fatal("Fork call error");
			case 0:
				planeProcess(createPlane(i), pipes[i]);
		}
	}
	return pipes;
}



