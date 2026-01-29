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

GSBool GSNetBaseHasData(int fd) {
	fd_set	       fds;
	struct timeval tv;

	tv.tv_sec  = 0;
	tv.tv_usec = 0;

	FD_ZERO(&fds);

	FD_SET(fd, &fds);

	select(FD_SETSIZE, &fds, NULL, NULL, &tv);

	if(FD_ISSET(fd, &fds)) return GSTrue;

	return GSFalse;
}

int GSNetBaseServer(int port) {
	int		   fd;
	struct sockaddr_in addr;
	int		   yes = 1;

	if((fd = new_socket()) < 0) return -1;

	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));

	addr.sin_family	     = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port	     = htons(port);

	if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		GSNetBaseClose(fd);
		return -1;
	}

	return fd;
}

int GSNetBaseClient(const char* hostname, int port, GSNetAddress* address) {
	struct sockaddr_in addr;
	int		   fd;

	addr.sin_family	     = AF_INET;
	addr.sin_addr.s_addr = inet_addr(hostname);
	addr.sin_port	     = htons(port);

	if(addr.sin_addr.s_addr == 0xffffffff) {
		struct hostent* host = gethostbyname(hostname);
		if(host == NULL) return -1;

		addr.sin_addr.s_addr = *(GSU32*)host->h_addr_list[0];
	}

	if(address != NULL){
		address->address = addr.sin_addr.s_addr;
		address->port	 = addr.sin_port;
	}

	if((fd = new_socket()) < 0) return -1;

	connect(fd, (struct sockaddr*)&addr, sizeof(addr));

	return fd;
}

void GSNetBaseClose(int fd) {
#ifdef _WIN32
	closesocket(fd);
#else
	close(fd);
#endif
}

int GSNetBaseRead(int fd, void* data, int size, GSNetAddress* address) {
	struct sockaddr_in addr;
	int		   len = sizeof(addr);
	int		   n;

	n = recvfrom(fd, data, size, 0, (struct sockaddr*)&addr, &len);

	address->address = addr.sin_addr.s_addr;
	address->port	 = addr.sin_port;

	return n;
}

void GSNetBaseWrite(int fd, void* data, int size, GSNetAddress* address) {
	struct sockaddr_in addr;

	addr.sin_family	     = AF_INET;
	addr.sin_addr.s_addr = address->address;
	addr.sin_port	     = address->port;

	sendto(fd, data, size, 0, (struct sockaddr*)&addr, sizeof(addr));
}
