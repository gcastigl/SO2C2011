#ifndef MYIPC_H_
#define MYIPC_H_

#include "ipc.h"
#include <sys/msg.h>
#include <errno.h>

int ipc_init(int key, int flags);

int ipc_write(int ipcId, IpcPackage* msg);

IpcPackage* ipc_get(int ipcId);

void ipc_close(int ipcId);

#endif

