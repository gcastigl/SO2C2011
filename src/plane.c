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
	close(wrPipe[READ]);
	close(rdPipe[WRITE]);
	ipcMessage *data = malloc(sizeof(ipcMessage));
	for (count = 0; count < 2; count++) {
		data->id = 1; //define with plane
		strcpy(data->message, "Buenas!\n");
		write(wrPipe[WRITE], data, PACKAGE_SIZE);
		sleep(getpid() % 4); // random sleep...
		if (read(rdPipe[READ], data, PACKAGE_SIZE) > 0) {
			printf("Message from airline -- %s\n", data->message);
		}
	}
	write(wrPipe[WRITE], "0", 1);
	exit(0);	
}

