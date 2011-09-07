#ifndef MYIPC_H_
#define MYIPC_H_

#include "ipc.h"
#include "common.h"
#include "map.h"
#include <sys/msg.h>
#include <errno.h>

#define SEM_FLAGS	0666
#define DATA_SIZE	128

void ipc_init(int id);

int ipc_write(int myId, int toId, char *msg);

int ipc_read(int myId, int fromId, char *msg);

int ipc_close(int id);

#endif
