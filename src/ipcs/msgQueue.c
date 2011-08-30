#include "communicator.h"

int ipc_init(int key, int flags) {
	key_t ipcId;
	ipcId = ftok("/tmp", key);
	if (ipcId == (key_t)-1) {
		return -1;
	}
	int id = msgget(key, flags | IPC_CREAT);
	if (id < 0) {
		return -1;
	}
	return id;
}

int ipc_get(int key) {
	return ipc_init(key, 0666);
}

int ipc_write(int ipcId, IpcPackage* msg) {
	int result = msgsnd(ipcId, (void *) msg, sizeof(msg->data) + sizeof(msg->sender), IPC_NOWAIT);
	if (result == -1) {
		perror("msgsnd failed @msjQueue/write");
	}
	return result;
}

IpcPackage* ipc_read(int ipcId, int fromId) {
	IpcPackage* msg = malloc(sizeof(IpcPackage));
	int result = msgrcv(ipcId, msg, sizeof(msg->data)+ sizeof(msg->sender), fromId, IPC_NOWAIT);
	if (result < 0) {
		free(msg);
		perror("msgrcv() failed @msjQueue/read, retuning NULL...");
		return NULL;
	}
	return msg;
}

void ipc_close(int ipcId) {
	msgctl(ipcId, IPC_RMID, (struct msqid_ds *) NULL);
}
