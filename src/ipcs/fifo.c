#include "communicator.h"

#define MSG_SIZE (sizeof(IpcPackage) - sizeof(long int))

int ipc_init(int key, int flags) {
	key_t ipcId;
	ipcId = ftok(TMP_FOLDER, key);
	if (ipcId == (key_t)-1) {
		return -1;
	}
	int id = msgget(key, flags);
	if (id < 0) {
		return -1;
	}
	return id;
}

int ipc_get(int key) {
	return ipc_init(key, IPC_CREAT | 0666);
}

int ipc_write(int ipcId, IpcPackage* msg) {
	return msgsnd(ipcId, msg, MSG_SIZE, IPC_NOWAIT);
}

int ipc_read(int ipcId, int fromId, IpcPackage* msg) {
	int result = msgrcv(ipcId, (void *) msg, MSG_SIZE, fromId, IPC_NOWAIT);
	return result;
}

void ipc_close(int ipcId) {
	msgctl(ipcId, IPC_RMID, (struct msqid_ds *) NULL);
}
