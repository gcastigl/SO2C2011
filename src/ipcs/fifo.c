#include "ipcs/fifo.h"

#define IPC_FIFO_DIR "./sofifo/"
int getFd(int id1, int id2, int flags);

int ipc_init(int myId, int size) {
	return mkdir(IPC_FIFO_DIR, 0777);
}

int getFd(int id1, int id2, int flags) {
	char filename[MAX_NAME_LENGTH];
	sprintf(filename, "%s%d_%d", IPC_FIFO_DIR, id1, id2);
	mkfifo(filename, 0777);
	return open(filename, flags);
}

int ipc_write(int myId, int toId, char *msg) {
	return write(getFd(myId, toId, O_WRONLY | O_NONBLOCK), msg, DATA_SIZE);
}

int ipc_read(int myId, int fromId, char *msg) {
	return read(getFd(fromId, myId, O_RDWR | O_NONBLOCK), msg, DATA_SIZE);
}

int ipc_close(int id) {
	DIR *dp;
	char *fileName;
	struct dirent *ep;
	dp = opendir(IPC_FIFO_DIR);
	if (dp != NULL) {
		while ((ep = readdir(dp))) {
			fileName = (char*)malloc((strlen(IPC_FIFO_DIR) + strlen(ep->d_name) + 1) * sizeof(char));
			sprintf(fileName, "%s%s", IPC_FIFO_DIR, ep->d_name);
			unlink(fileName);
			free(fileName);
		}
		closedir(dp);
	}
	return rmdir(IPC_FIFO_DIR);
}
