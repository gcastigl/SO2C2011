#ifndef MYIPC_H_
#define MYIPC_H_

int ipc_init(int ipcId, int size);

int ipc_connect(int ipcId, int index);

int ipc_disconnect(int ipcId, int index);

void ipc_close(int ipcId);

int ipc_write(int ipcId, int targetId, const char* msg, int msgSize);

char* ipc_get(int ipcId, int targetId);
