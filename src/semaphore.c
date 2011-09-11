#include "semaphore.h"

int semaphore_operation(int id, int op, int semnum);

/*
 * Gets the semaphore for the key specified and initializes
 * all places with 0.
 */
int semaphore_create(int key, int semSize, int flags) {
    int semid;
    key += MAGIC_NUMBER;
    semid = semget(key, semSize, flags | IPC_CREAT | IPC_EXCL);

    if (semid == -1) {
        
        if (errno == EEXIST) {
            semid = semget(key, semSize, flags | IPC_CREAT);
        }
        if (semid == -1) {
            log_error("Couldn't create semaphore %d, cause %d: %s\n", key, errno, strerror(errno));
            return -1;
        }
    }
    semaphore_setAll(semid, semSize, 0);
    return semid;
}

int semaphore_get(int key) {
    key += MAGIC_NUMBER;
    int ret = semget(key, 0, 0666 | IPC_CREAT);
    if (ret < 0) {
        switch(errno) {
            case EACCES:
            fprintf(stderr, "EACCES");
            break;
            case EEXIST:
            fprintf(stderr, "EEXIST");
            break;
            case EIDRM:
            fprintf(stderr, "EIDRM");
            break;
            case ENOENT:
            fprintf(stderr, "ENOENT");
            break;
            case ENOMEM:
            fprintf(stderr, "ENOMEM");
            break;
            case ENOSPC:
            fprintf(stderr, "ENOSPC");
            break;
        }
        log_error("Error getting semaphore");
    }
    return ret;
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
