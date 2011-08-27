#include "../include/company.h"
#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

void wakeUpPlanes(Company* airline, int semId);

Company *newCompany(char* name, int maxPlaneCount) {
	Company* company = malloc(sizeof(Company));
	company->name = name;
	company->planeCount = 0;
	company->plane = malloc(maxPlaneCount * sizeof(Company));
	return company;
}

void addPlane(Company *company, Plane plane) {
	company->plane[company->planeCount++] = plane;
}

void companyStart(Company* Company) {
	int i;
	int semId = semaphore_create(33, Company->planeCount, 0);
	if (semId == -1) {
			fatal("Error creating semaphore");
	}
	for(i = 0; i < Company->planeCount; i++) {
		semctl(semId, i, SETVAL, 0);
		pthread_create(&(Company->plane[i].thread), NULL, planeStart, Company->plane + i);
		//int pthread_key_create(pthread_key_t *key, void (*destructor) (void *));
	}
	for (i = 0; i < 10; i++) {
		printf("Semaphore increment\n");
		wakeUpPlanes(Company, semId);
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
		for (i = 0; i < Company->planeCount; i++) {
			if (ipcIsReady(i, WRITE) == TRUE) {
				strcpy(package.data, "Message to plane :D\n");
				sendData(i, package);
			}
		}
		sleep(1);
	}*/
    exit(0);
}

void wakeUpPlanes(Company* Company, int semId) {
	int i;
	for(i = 0; i < Company->planeCount; i++) {
		semaphore_increment(semId, i);
	}
}
