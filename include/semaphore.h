#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "common.h"
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>

#define MAGIC_NUMBER 50 // O.o

//#ifdef LINUX
typedef union {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
} semun;
//#endif

int semaphore_create(int key, int semSize, int flags);

int semaphore_get(int key);

int semaphore_increment(int id, int semnum);

int semaphore_decrement(int id, int semnum);

int semaphore_setAll(int id, int semSize, int value);

int semaphore_operation(int id, int op, int semnum);

int semaphore_destroy(int id);

int semaphore_getAll(int semId, int semSize, unsigned short* values);

#endif
