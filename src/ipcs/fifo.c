#include "ipcs/fifo.h"

#define IPC_FIFO_DIR "./sofifo/"
int getFd(int id1, int id2, int flags);

int ipc_init(int myId, int size) {
	return mkdir(IPC_FIFO_DIR, 0777);
}

int getFd(int id1, int id2, int flags) {
	char filename[MAX_NAME_LENGTH];
	sprintf(filename, "%s%d_%d", IPC_FIFO_DIR, id1, id2);
	mkfifo(filename, 0777 | IPC_CREAT);
	return open(filename, flags| IPC_CREAT);
}

int ipc_write(int myId, int toId, char *msg) {
	return write(getFd(myId, toId, O_WRONLY | O_NONBLOCK), msg, DATA_SIZE);
}

int ipc_read(int myId, int fromId, char *msg) {
	return read(getFd(fromId, myId, O_RDWR | O_NONBLOCK), msg, DATA_SIZE);
}

int ipc_close(int id) {
	DIR *dp;
	char *fileName = malloc(strlen(IPC_FIFO_DIR) + 50 * sizeof(char));
	sprintf(fileName, "%s%d_%d", IPC_FIFO_DIR, serverId, id);
	unlink(fileName);
	free(fileName);
	sprintf(fileName, "%s%d_%d", IPC_FIFO_DIR, id, serverId);
	unlink(fileName);
	free(fileName);
	return 0;
}
