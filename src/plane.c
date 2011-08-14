#include "../include/plane.h"

Plane* createPlane(int id) {
	Plane* plane = (Plane*) malloc(sizeof(Plane));
	plane->id = id;
	plane->distanceLeft = -1;
	plane->targetId = -1;
	plane->supplies = NULL;
	return plane;
}

void planeProcess(Plane* plane, int* wrPipe, int* rdPipe) {
	int count;
	char buf[2];
	close(wrPipe[READ]);
	close(rdPipe[WRITE]);
	for (count = 0; count < 2; count++) {
		char * msj = "9";
		write(wrPipe[WRITE], msj, 1);
		sleep(getpid() % 4); // random sleep...
		if (read(rdPipe[READ], buf, 1) > 0) {
			printf("Message from airline -- %c\n", buf[0]);
		}
	}
	write(wrPipe[WRITE], "0", 1);
	exit(0);	
}

