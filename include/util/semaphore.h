#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "common.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <semaphore.h>

#define MAGIC_NUMBER 50 // O.o

//#ifdef LINUX
typedef union {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
} semun;
//#endif

sem_t* semaphore_create(char *name);
sem_t* sem_get(char *name);
#endif
