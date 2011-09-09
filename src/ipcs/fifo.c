#include "communicator.h"
#include "util/logger.h"
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#define MSG "%03d_%03d_%03d\n"
#define MSG_LEN 3+1+3+1+3+1+1
#define IPC_FIFO_DIR "/tmp/sofifo/"

int init_ipc(int myId, int size) {
	return mkdir(IPC_FIFO_DIR, 0777);
}

int getFd(int id1, int id2, int flags) {
	char filename[MAX_NAME_LENGTH];
	sprintf(filename, "%s%d_%d", IPC_FIFO_DIR, id1, id2);
	if (mkfifo(filename, 0777) != -1) {
		return open(filename, flags);
	}
	return -1;
}

int ipc_write(int myId, int toId, char *msg) {
	return write(getFd(myId, toId, O_WRONLY | O_NONBLOCK), msg, MSG_LEN);
}

int ipc_read(int myId, int fromId, char *msg) {
	return read(getFd(fromId, myId, O_RDWR), msg, MSG_LEN);
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
