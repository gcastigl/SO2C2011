#include "../include/airline.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>

void createPlanes(Airline* airline);
void wakeUpPlanes(Airline* airline, int semId);

Airline* createAirline(long id, Map* map, int numberOfPlanes) {
	Airline* airline = (Airline*) malloc(sizeof(Airline));
	airline->id = id;
	airline->map = map;
	airline->planes = malloc(sizeof(Plane) * numberOfPlanes);
	airline->planesThreads = malloc(sizeof(pthread_t) * numberOfPlanes);
	airline->planesSize = numberOfPlanes;
	createPlanes(airline);
	return airline;
//  ipcSetup(numberOfPlanes);
//  ipcPostChildSetup(numberOfPlanes);
}

//TODO: agregar items a los aviones recien creados
void createPlanes(Airline* airline) {
	int i;
	Plane* newPlane;
	for (i = 0; i < airline->planesSize; i++) {
		newPlane = createPlane(airline->map, i, 0, NULL, 0);
		airline->planes[i] = *newPlane;
	}
}

void airlineStart(Airline* airline) {
	int i;
	int semId = semaphore_create(33, airline->planesSize, 0);
	if (semId == -1) {
		fatal("Error creating semaphore");
	}
	for(i = 0; i < airline->planesSize; i++) {
		semctl(semId, i, SETVAL, 0);
		pthread_create(airline->planesThreads + i, NULL, planeStart, airline->planes + i);
	}
	for (i = 0; i < 10; i++) {
		printf("Semaphore increment\n");
		wakeUpPlanes(airline, semId);
		sleep(1);
	}
	/*
	FakeIPC fake_ipc;
	fake_ipc.counter = 0;
	fake_ipc.bufferSize = 1000;
	fake_ipc.buffer = malloc(sizeof(int) * fake_ipc.bufferSize);
	
	pthread_mutex_init(&fake_ipc.mutexCounter, NULL);
	pthread_mutex_init(&fake_ipc.finished_thinking_mutex, NULL);
	pthread_t idHilo;
	pthread_create(&idHilo, NULL, planeStart, &fake_ipc);
	while (1) {
		pthread_mutex_lock(&fake_ipc.mutexCounter);
		
		for (i = 0; i < fake_ipc.bufferSize; i++)
			fake_ipc.buffer[i] = fake_ipc.counter;
			
		pthread_mutex_unlock(&fake_ipc.mutexCounter);
		fake_ipc.counter++;
	}*/
    /*int i;
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
	}*/
    exit(0);
}

void wakeUpPlanes(Airline* airline, int semId) {
	int i;
	for(i = 0; i < airline->planesSize; i++) {
		semaphore_increment(semId, i);
	}
}
