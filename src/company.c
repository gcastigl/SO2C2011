#include "../include/company.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>

Company * newCompany(char* name, int maxPlaneCount) {
	Company* company = malloc(sizeof(Company));
	company->name = name;
	company->planeCount = 0;
	company->plane = malloc(maxPlaneCount * sizeof(Company));
	return company;
}

void addPlane(Company *company, Company plane) {
	company->plane[company->planeCount++] = plane;
}

void wakeUpPlanes(Company* airline, int semId);

Company* createAirline(long id, Map* map, int numberOfPlanes) {
	Company* airline = (Company*) malloc(sizeof(Company));
	airline->id = id;
	airline->map = map;
	airline->planes = malloc(sizeof(Company) * numberOfPlanes);
	airline->planesThreads = malloc(sizeof(pthread_t) * numberOfPlanes);
	airline->planesSize = numberOfPlanes;
	createPlanes(airline);
	printMatrix(map->distances, map->citiesSize, map->citiesSize);
	return airline;
//  ipcSetup(numberOfPlanes);
//  ipcPostChildSetup(numberOfPlanes);
}

void airlineStart(Company* airline) {
	int i;
	int semId = semaphore_create(33, airline->planesSize, 0);
	if (semId == -1) {
		fatal("Error creating semaphore");
	}
	for(i = 0; i < airline->planesSize; i++) {
		semctl(semId, i, SETVAL, 0);
		pthread_create(airline->planesThreads + i, NULL, planeStart, airline->planes + i);
		//TODO: instaed of NULL, a void* can be called to see the exit status...
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

void wakeUpPlanes(Company* airline, int semId) {
	int i;
	for(i = 0; i < airline->planesSize; i++) {
		semaphore_increment(semId, i);
	}
}
