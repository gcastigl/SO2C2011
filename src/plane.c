#include "../include/plane.h"

Plane* createPlane(int id) {
	Plane* plane = (Plane*) malloc(sizeof(Plane));
	plane->id = id;
	plane->distanceLeft = -1;
	plane->targetId = -1;
	plane->supplies = NULL;
	return plane;
}

void planeProcess(Plane* plane, int* wrPipe, int** rdPipe) {
	int count;
	close(wrPipe[READ]);
	for (count = 0; count < 2; count++) {
		char * msj = "This is plane xx... over!\n";
		write(wrPipe[WRITE], msj, 20);
		sleep(getpid() % 4); // random sleep...
	}
	write(wrPipe[WRITE], "I'm out!!", 10);
	exit(0);	
}

