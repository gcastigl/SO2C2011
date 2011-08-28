#include "../include/company.h"

void wakeUpPlanes(Company* airline, int semId);
void waitUntilPlanesReady(Company* company, int semId);

Company *newCompany(char* name, int maxPlaneCount) {
	Company* company = malloc(sizeof(Company));
	company->name = name;
	company->planeCount = 0;
	company->plane = malloc(maxPlaneCount * sizeof(Company));
	for (int i = 0; i < maxPlaneCount; ++i) {
		addPlane(company, newPlane(i));
	}
	return company;
}

void addPlane(Company *company, Plane* plane) {
	company->plane[company->planeCount++] = *plane;
}

void companyStart(Company* company) {
	int planesTurnSemId = semaphore_create(SEM_PLANE_KEY, company->planeCount, 0600);
	int companyTurnSemId = semaphore_create(SEM_COMPANY_KEY, 1, 0600);
	if (planesTurnSemId <= 0 || companyTurnSemId <= 0) {
		fatal("Error creating semaphore");
	}
	for(int i = 0; i < company->planeCount; i++) {
		semctl(planesTurnSemId, i, SETVAL, 0);
		pthread_create(&(company->plane[i].thread), NULL, planeStart, company->plane + i);
	}
	for (int i = 0; i < 10; i++) {
		wakeUpPlanes(company, planesTurnSemId);
		waitUntilPlanesReady(company, companyTurnSemId);
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

void wakeUpPlanes(Company* company, int semId) {
	printf("planes wake up!\n");
	for(int i = 0; i < company->planeCount; i++) {
		semaphore_increment(semId, i);
	}
}

void waitUntilPlanesReady(Company* company, int semId) {
	for(int i = 0; i < company->planeCount; i++) {
		semaphore_decrement(semId, 0);
	}
	printf("Waiting done!...\n");
}
