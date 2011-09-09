#include "communicator.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void ipc_init(int id) {
	// Read, write, and execute by owner
	mkfifo(TMP_FOLDER, S_IRWXU);
}

int ipc_write(int myId, int toId, char *msg) {
	char filename[MAX_NAME_LENGTH];
	sprintf(filename, "%sfifo_%d_%d", TMP_FOLDER,myId, toId);
	log_debug("filename: %s", filename);
	return -1;
}

int ipc_read(int myId, int fromId, char *msg) {
	return -1;
}

int ipc_close(int id) {
	return -1;
}

/*
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
}*/
