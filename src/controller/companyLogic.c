#include "controller/companyLogic.h"

void wakeUpPlanes(Company* company, int semId);
void waitUntilPlanesReady(Company* company, int semId);
void readAndProcessMessages(Company *company);
void sig_threadHandler();

//TODO: when everything is working as it should be, all the sprnfs calls should be removed from the code

/*
 * 1 - Wake up planes.
 * 2 - Read & process plane messages.
 */
void companyStart(Company* company) {
    sigset_t signal_set;
    pthread_t sig_thread;
    
    sigfillset(&signal_set);
    pthread_sigmask(SIG_BLOCK, &signal_set, NULL);
	int planesTurnSemId = semaphore_create(SEM_PLANE_KEY, company->planeCount, 0666);
	int companyTurnSemId = semaphore_create(SEM_COMPANY_KEY, 1, 0666);
	int ipcId = ipc_init(IPC_KEY, IPC_CREAT | 0666);
	if (planesTurnSemId < 0 || companyTurnSemId < 0 || ipcId < 0) {
		fatal("Error creating semaphore");
	}
	for(int i = 0; i < company->planeCount; i++) {
		pthread_create(&(company->plane[i]->thread), NULL, planeStart, company->plane[i]);
	}
    pthread_create(&sig_thread, NULL, sig_threadHandler, NULL);
	for (int i = 0; i < 5; i++) {
		wakeUpPlanes(company, planesTurnSemId);
		waitUntilPlanesReady(company, companyTurnSemId);
		readAndProcessMessages(company);
	}
    exit(0);
}

void wakeUpPlanes(Company* company, int semId) {
	for(int i = 0; i < company->planeCount; i++) {
		semaphore_increment(semId, company->plane[i]->id - MIN_PLANE_ID);
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
	char auxBuffer[100];
	for (int i = 0; i < company->planeCount; i++) {
		int msgRead = ipc_read(ipcId, company->id, msg);
		if (msgRead != -1) {
			log_debug("[Company %d] Message from child %ld -> %s", company->id, msg->sender, msg->data);
			log_debug("[Company %d] writing response to: %ld\n", company->id, msg->sender);
			// Set un msg with the new data to be sent
			msg->addressee = msg->sender;
			msg->sender = company->id;
			strcpy(msg->data, "This is a response with data from the company\n");
			int writeReturn = ipc_write(ipcId, msg);
			if (writeReturn != -1) {
				log_debug("[Message sent OK]\n");
			} else {
				log_debug("[Company %d] ERROR writing to plane...\n", company->id);
				perror("");
			}
		} else {
			log_debug("[Company %d] No message from child: %d\n", company->id, company->plane[i]->id);
		}
	}
}

void sig_threadHandler() {
    sigset_t signal_set;
    int sig;
    for (;;) {
        sigfillset(&signal_set);
        sigwait(&signal_set, &sig);
        log_debug("Signal thead caught signal number %d\n", sig);
        childSignalHandler(sig);
    }
}