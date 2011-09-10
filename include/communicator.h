#ifndef COMMUNICATOR_H_
#define COMMUNICATOR_H_

#include "common.h"
#include <errno.h>

#define SEM_FLAGS	0666
#define IPC_FLAGS	0666
#define DATA_SIZE	1024

#define READ 0
#define WRITE 1

int init_ipc(int myId, int size);

int ipc_write(int myId, int toId, char *msg);

int ipc_read(int myId, int fromId, char *msg);

int ipc_close(int id);

#endif
