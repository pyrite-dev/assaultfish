#include <GearSrc/Net.h>

#ifdef _WIN32
/* TODO: check if this works */
#include <winsock.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <netdb.h>
#endif

static int new_socket(void) {
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

#ifdef _WIN32
	if(fd == INVALID_SOCKET) return -1;
#else
	if(fd < 0) return -1;
#endif

	return fd;
}

GSBool GSNetBaseHasData(GSNetSocket sock) {
	fd_set	       fds;
	struct timeval tv;

	tv.tv_sec  = 0;
	tv.tv_usec = 0;

	FD_ZERO(&fds);

	FD_SET(sock->fd, &fds);

	select(FD_SETSIZE, &fds, NULL, NULL, &tv);

	if(FD_ISSET(sock->fd, &fds)) return GSTrue;

	return GSFalse;
}

GSNetSocket GSNetBaseServer(int port) {
	GSNetSocket	   sock;
	int		   fd;
	struct sockaddr_in addr;
	int		   yes = 1;

	if((fd = new_socket()) < 0) return NULL;

	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));

	addr.sin_family	     = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port	     = htons(port);

	sock	     = malloc(sizeof(*sock));
	sock->fd     = fd;
	sock->client = 0;

	if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		GSNetBaseClose(sock);
		return NULL;
	}

	return sock;
}

GSNetSocket GSNetBaseClient(const char* hostname, int port, GSNetAddress* address) {
	GSNetSocket	   sock;
	int		   fd;
	struct sockaddr_in addr;

	addr.sin_family	     = AF_INET;
	addr.sin_addr.s_addr = inet_addr(hostname);
	addr.sin_port	     = htons(port);

	if(addr.sin_addr.s_addr == 0xffffffff) {
		struct hostent* host = gethostbyname(hostname);
		if(host == NULL) return NULL;

		addr.sin_addr.s_addr = *(GSU32*)host->h_addr_list[0];
	}

	if(address != NULL) {
		address->address = addr.sin_addr.s_addr;
		address->port	 = addr.sin_port;
	}

	if((fd = new_socket()) < 0) return NULL;

	sock	     = malloc(sizeof(*sock));
	sock->fd     = fd;
	sock->client = 1;

	connect(fd, (struct sockaddr*)&addr, sizeof(addr));

	return sock;
}

void GSNetBaseClose(GSNetSocket sock) {
#ifdef _WIN32
	closesocket(sock->fd);
#else
	close(sock->fd);
#endif
	free(sock);
}

int GSNetBaseRead(GSNetSocket sock, void* data, int size, GSNetAddress* address) {
	int n;

	if(sock->client) {
		n = recv(sock->fd, data, size, 0);
	} else {
		struct sockaddr_in addr;
		int		   len = sizeof(addr);

		n = recvfrom(sock->fd, data, size, 0, (struct sockaddr*)&addr, &len);

		address->address = addr.sin_addr.s_addr;
		address->port	 = addr.sin_port;
	}

	return n;
}

void GSNetBaseWrite(GSNetSocket sock, void* data, int size, GSNetAddress* address) {
	if(sock->client) {
		send(sock->fd, data, size, 0);
	} else {
		struct sockaddr_in addr;

		addr.sin_family	     = AF_INET;
		addr.sin_addr.s_addr = address->address;
		addr.sin_port	     = address->port;

		sendto(sock->fd, data, size, 0, (struct sockaddr*)&addr, sizeof(addr));
	}
}
