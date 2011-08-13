#include "../include/plane.h"

Plane* createPlane(int id) {
	Plane* plane = (Plane*) malloc(sizeof(Plane));
	plane->id = id;
	plane->distanceLeft = -1;
	plane->targetId = -1;
	plane->supplies = NULL;
	return plane;
}

void planeProcess(Plane* plane, int* pipe) {
	int count;
	close(pipe[0]);
	for (count = 0; count < 2; count++) {
		char * msj = "This is plane xx... over!\n";
		write(pipe[1], msj, 20);
		sleep(getpid() % 4); // random sleep...
	}
	write(pipe[1], "I'm out!!", 10);
	exit(0);	
}

