#include "controller/companyLogic.h"

void wakeUpPlanes(Company* company, int semId);

void waitUntilPlanesReady(Company* company, int semId);

void readAndProcessMessages(Company *company);

/*
 * 1 - Wake up planes.
 * 2 - Read & process plane messages.
 */
void companyStart(Company* company) {
	int planesTurnSemId = semaphore_create(SEM_PLANE_KEY, company->planeCount, 0666);
	int companyTurnSemId = semaphore_create(SEM_COMPANY_KEY, 1, 0666);
	if (planesTurnSemId <= 0 || companyTurnSemId <= 0) {
		fatal("Error creating semaphore");
	}
	for(int i = 0; i < company->planeCount; i++) {
		semctl(planesTurnSemId, i, SETVAL, 0);
		pthread_create(&(company->plane[i]->thread), NULL, planeStart, company->plane[i]);
	}
	semctl(companyTurnSemId, 0, SETVAL, 0);
	for (int i = 0; i < 10; i++) {
		wakeUpPlanes(company, planesTurnSemId);
		waitUntilPlanesReady(company, companyTurnSemId);
		readAndProcessMessages(company);
	}
    exit(0);
}

void wakeUpPlanes(Company* company, int semId) {
	printf("planes wake up!\n\n");
	for(int i = 0; i < company->planeCount; i++) {
		semaphore_increment(semId, i);
	}
}

void waitUntilPlanesReady(Company* company, int semId) {
	for(int i = 0; i < company->planeCount; i++) {
		semaphore_decrement(semId, 0);
	}
	printf("Waiting done!...\n\n");
}

void readAndProcessMessages(Company *company) {
	IpcPackage package;
	int ipcId = ipc_get(IPC_KEY);
	for (int i = 0; i < company->planeCount; i++) {
		IpcPackage * msg = ipc_read(ipcId, company->id);
		if (msg != NULL) {
			printf("Message from child %ld -> %s\n", company->plane[i]->thread, msg->data);
			package.addressee = company->id;
			strcpy(package.data, "This is a response message from the company!\n");
			ipc_write(ipcId, &package);
		} else {
			printf("No message from child: %d\n", company->plane[i]->id);
		}
	}
}

/*
FakeIPC fake_ipc;
fake_ipc.buffer = malloc(sizeof(int) * fake_ipc.bufferSize);
pthread_mutex_init(&fake_ipc.mutexCounter, NULL);
pthread_mutex_init(&fake_ipc.finished_thinking_mutex, NULL);
pthread_create(&idHilo, NULL, planeStart, &fake_ipc);
while (1) {
	pthread_mutex_lock(&fake_ipc.mutexCounter);
	...
	pthread_mutex_unlock(&fake_ipc.mutexCounter);
}*/
