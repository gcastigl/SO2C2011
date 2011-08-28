#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <sys/sem.h>

#ifdef LINUX
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};
#endif

int semaphore_create(int key, int semSize, int flags);

int semaphore_increment(int id, int semnum);

int semaphore_decrement(int id, int semnum);

int semaphore_operation(int id, int op, int semnum);

#endif
