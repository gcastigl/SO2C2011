#include "semaphore.h"

int semaphore_operation(int id, int op, int semnum);

/*
 * Gets the semaphore for the key specified and initializes
 * all places with 0.
 */
int semaphore_create(int key, int semSize, int flags) {
	key_t semKey = ftok(TMP_FOLDER, key);
	if (semKey == (key_t)-1) {
		return -1;
	}
	int semId = semget(semKey, semSize, 0666 | IPC_CREAT);
	if (semId == -1) {
		return -1;
	}
	// Set all semaphores to 0
	semaphore_setAll(semId, semSize, 0);
	return semId;
}

int semaphore_get(int key) {
	key_t semKey = ftok(TMP_FOLDER, key);
	if (semKey == (key_t)-1) {
		return -1;
	}
	return semget(semKey, 1, 0);
}

int semaphore_setAll(int id, int semSize, int value) {
    unsigned short semValues[semSize];
    for (int i = 0; i < semSize; i++) {
    	semValues[i] = value;
    }
    semun sem_union = {.array = semValues};
    return semctl(id, 0, SETALL, sem_union);
}

int semaphore_increment(int id, int semnum) {
	return semaphore_operation(id, 1, semnum);
}

int semaphore_decrement(int id, int semnum) {
	return semaphore_operation(id, -1, semnum);
}

int semaphore_operation(int id, int op, int semnum) {
	struct sembuf operation;
	operation.sem_num = semnum;
	operation.sem_op = op;
	operation.sem_flg = 0;
	return semop(id, &operation, 1);
}

int semaphore_destroy(int id) {
	return semctl(id, 0, IPC_RMID);
}
