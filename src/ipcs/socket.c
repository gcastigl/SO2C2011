#include "ipcs/socket.h"

#define IPC_SOCKET_DIR "/tmp/sosocket/"
#define ADDR_SIZE sizeof(struct sockaddr_un)
typedef struct {
	int sockfd;
	struct sockaddr_un *addr;
} socket_t;

socket_t *getSocket(int id1, int id2);

int ipc_init(int myId, int size) {
	return mkdir(IPC_SOCKET_DIR, 0777);
}

socket_t *getSocket(int id1, int id2) {
	socket_t *sock = (socket_t*) malloc(sizeof(socket_t));
	char path[MAX_NAME_LENGTH];
	sprintf(path, "%s%d_%d", IPC_SOCKET_DIR, id1, id2);
	sock->sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	sock->addr->sun_family = AF_UNIX;
	strcpy(sock->addr->sun_path, path);
	bind(sock->sockfd, (struct sockaddr *)sock->addr, ADDR_SIZE);
	return sock;
}

int ipc_write(int myId, int toId, char *msg) {
	socket_t *socket = getSocket(myId, toId);
	return sendto(socket->sockfd, msg, DATA_SIZE, 0 , (struct sockaddr *)socket->addr, ADDR_SIZE);
}

int ipc_read(int myId, int fromId, char *msg) {
	socket_t *socket = getSocket(fromId, myId);
	return recvfrom(socket->sockfd, *msg, DATA_SIZE, 0, NULL, NULL);
}

int ipc_close(int id) {
	DIR *dp;
	char *fileName;
	struct dirent *ep;
	dp = opendir(IPC_SOCKET_DIR);
	if (dp != NULL) {
		while ((ep = readdir(dp))) {
			fileName = (char*)malloc((strlen(IPC_SOCKET_DIR) + strlen(ep->d_name) + 1) * sizeof(char));
			sprintf(fileName, "%s%s", IPC_SOCKET_DIR, ep->d_name);
			unlink(fileName);
			free(fileName);
		}
		closedir(dp);
	}
	return rmdir(IPC_SOCKET_DIR);
}
