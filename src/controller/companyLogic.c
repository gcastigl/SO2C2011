#include "controller/companyLogic.h"

void wakeUpPlanes(Company* company, int semId);
void waitUntilPlanesReady(Company* company, int semId);
void readAndProcessMessages(Company *company);

//TODO: when everything is working as it should be, all the sprnfs calls should be removed from the code

/*
 * 1 - Wake up planes.
 * 2 - Read & process plane messages.
 */
void companyStart(Company* company) {
    printf("Creating company...\n");
    log_debug("Creating company...\n");
	int planesTurnSemId = semaphore_create(SEM_PLANE_KEY, company->planeCount, 0666);
	int companyTurnSemId = semaphore_create(SEM_COMPANY_KEY, 1, 0666);

	for(int i = 0; i < company->planeCount; i++) {
		pthread_create(&(company->plane[i]->thread), NULL, planeStart, company->plane[i]);
	}

	for (int i = 0; i < 5; i++) {
		wakeUpPlanes(company, planesTurnSemId);
		waitUntilPlanesReady(company, companyTurnSemId);
		readAndProcessMessages(company);
	}
    exit(0);
}

void wakeUpPlanes(Company* company, int semId) {
	for(int i = 0; i < company->planeCount; i++) {
		semaphore_increment(semId, PLANE_INDEX(company->plane[i]->id));
	}
}

void waitUntilPlanesReady(Company* company, int semId) {
	for(int i = 0; i < company->planeCount; i++) {
		semaphore_decrement(semId, 0);
	}
	printf("[Company %d] Waiting done!...\n", company->id);
}

void readAndProcessMessages(Company *company) {
	char* msg = malloc(sizeof(DATA_SIZE));
	for (int i = 0; i < company->planeCount; i++) {
		int msgRead = ipc_read(company->id, company->plane[i]->id, msg);
		if (msgRead != -1) {
			log_debug("[Company %d] Message from child %ld -> %s", company->id, company->plane[i]->id, msg);
			log_debug("[Company %d] writing response to: %ld\n", company->id, company->plane[i]->id);
			// Set un msg with the new data to be sent
			strcpy(msg, "This is a response with data from the company\n");
			int writeReturn = ipc_write(company->id, company->plane[i]->id, msg);
			if (writeReturn != -1) {
				log_debug("[Message sent OK]\n");
			} else {
				log_debug("[Company %d] ERROR writing to plane...\n", company->id);
				perror("readAndProcessMessages");
			}
		} else {
			log_debug("[Company %d] No message from child: %d\n", company->id, company->plane[i]->id);
		}
	}
}


