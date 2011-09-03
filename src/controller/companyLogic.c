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
	int ipcId = ipc_init(IPC_KEY, IPC_CREAT | 0666);
	if (planesTurnSemId < 0 || companyTurnSemId < 0 || ipcId < 0) {
		fatal("Error creating semaphore");
	}
	for(int i = 0; i < company->planeCount; i++) {
		pthread_create(&(company->plane[i]->thread), NULL, planeStart, company->plane[i]);
	}
	for (int i = 0; i < 5; i++) {
		printf("--------------- turn: %d -------------------\n", i);
		wakeUpPlanes(company, planesTurnSemId);
		waitUntilPlanesReady(company, companyTurnSemId);
		readAndProcessMessages(company);
	}
    exit(0);
}

void wakeUpPlanes(Company* company, int semId) {
	printf("[Company %d] Planes wake up!\n", company->id);
	for(int i = 0; i < company->planeCount; i++) {
		printf("[Company %d] Waking %d\n", company->id, company->plane[i]->id);
		semaphore_increment(semId, company->plane[i]->id);
	}
}

void waitUntilPlanesReady(Company* company, int semId) {
	for(int i = 0; i < company->planeCount; i++) {
		semaphore_decrement(semId, 0);
	}
	printf("[Company %d] Waiting done!...\n", company->id);
}

void readAndProcessMessages(Company *company) {
	IpcPackage* msg = malloc(sizeof(IpcPackage));
	int ipcId = ipc_get(IPC_KEY);
	for (int i = 0; i < company->planeCount; i++) {
		int msgRead = ipc_read(ipcId, company->id, msg);
		if (msgRead != -1) {
			printf("[Company %d] Message from child %ld -> %s", company->id, msg->sender, msg->data);
			printf("[Company %d] writing response to: %ld\n", company->id, msg->sender);
			// Set un msg with the new data to be sent
			msg->addressee = msg->sender;
			msg->sender = company->id;
			strcpy(msg->data, "This is a response with data from the company\n");
			int writeReturn = ipc_write(ipcId, msg);
			if (writeReturn != -1) {
				printf("[Message sent OK]\n");
			} else {
				printf("[Company %d] ERROR writing to plane...\n", company->id);
				perror("");
			}
		} else {
			printf("[Company %d] No message from child: %d\n", company->id, company->plane[i]->id);
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
