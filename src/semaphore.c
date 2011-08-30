#include "semaphore.h"

int semaphore_operation(int id, int op, int semnum);

int semaphore_create(int key, int semSize, int flags) {
	key_t semKey = ftok(DEFAULT_FOLDER, key); //FIXME: change to /tmp/sim
	if (semKey == (key_t)-1) {
		return -1;
	}
	int semId = semget(semKey, semSize, flags | IPC_CREAT);
	if (semId == -1) {
		return -1;
	}
	return semId;
}

int semaphore_get(int key) {
	return semaphore_create(key, 1, 0666 | IPC_CREAT);
	// This 1 (second parameter) does not matter because the semaphore is supposed to be already created.
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
	semctl(id, 0, IPC_RMID);
	return 0;
}
