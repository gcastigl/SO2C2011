#include "communicator.h"

#include <sys/msg.h>

typedef struct {
	long fromId;
	char data[DATA_SIZE];
} MsgQueuePackage;

#define MSG_SIZE (DATA_SIZE - sizeof(long int))

int ipc_get(int id);

MsgQueuePackage *newMsgQueuePackage(int id, char* data);

int init_ipc(int myId, int size) {
	return 0;
}

int ipc_get(int key) {
	key_t ipcId;
	ipcId = ftok(TMP_FOLDER, key);
	if (ipcId == (key_t)-1) {
		fatal("Error writing msg - ftok");
		return -1;
	}
	int id = msgget(ipcId, IPC_CREAT | SEM_FLAGS);
	if (id < 0) {
		fatal("Error writing msg - msgget");
		return -1;
	}
	return id;
}

int ipc_write(int myId, int toId, char *data) {
	int toIpcId = ipc_get(toId);
	MsgQueuePackage *msg = newMsgQueuePackage(myId, data);
	return msgsnd(toIpcId, (void*) msg, MSG_SIZE, IPC_NOWAIT);
}

int ipc_read(int myId, int fromId, char *data) {
	int myIpcId = ipc_get(myId);
	MsgQueuePackage msg;
	int result = msgrcv(myIpcId, (void *)&msg, MSG_SIZE, fromId, IPC_NOWAIT);
	if (result == -1) {
		return -1;
	}
	memcpy(data, msg.data, DATA_SIZE);
	return result;
}

int ipc_close(int id) {
	int ipcId = ipc_get(id);
	return msgctl(ipcId, IPC_RMID, (struct msqid_ds *) NULL);
}

MsgQueuePackage *newMsgQueuePackage(int id, char* data) {
	MsgQueuePackage* msg = malloc(sizeof(MsgQueuePackage));
	msg->fromId = id;
	memcpy(msg->data, data, DATA_SIZE);
	return msg;
}
