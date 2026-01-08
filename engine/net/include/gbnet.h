#ifndef __GBNET_H__
#define __GBNET_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <poll.h>

/* TCP */
#define NET_PORT 3219

#define NET_PING 3

enum gbnet_packet_type {
	NET_PACKET_PING = 0,
	NET_PACKET_PONG,
	NET_PACKET_USER
};

typedef struct gbnet_client	   gbnet_client_t;
typedef struct gbnet_server	   gbnet_server_t;
typedef struct gbnet_packet_header gbnet_packet_header_t;
typedef struct gbnet_packet	   gbnet_packet_t;

struct gbnet_client {
	int    fd;
	time_t last_ping;
	void (*on_packet)(gbnet_client_t* client, gbnet_packet_t* packet);
	void (*on_disconnect)(gbnet_client_t* client);
	void* user;
	struct {
		int sent_ping;
		int cleanup;
	} server;
};

struct gbnet_server {
	int		   fd;
	struct sockaddr_in address;
	gbnet_client_t*	   clients;
	void*		   user;
	void (*on_client)(gbnet_server_t* server, gbnet_client_t* client);
	void (*on_tick)(gbnet_server_t* server);
};

#pragma pack(1)
struct gbnet_packet_header {
	unsigned char  type;
	unsigned short length; /* ok, seriously, who sends 64k at ONCE */
};
#pragma pack()

/* !!! REMEMBER TO FREE DATA AFTER RECV UNLESS IT'S NULL !!! */
struct gbnet_packet {
	gbnet_packet_header_t header;
	void*		      data;
};

gbnet_client_t* gbnet_client(const char* host, int port);
void		gbnet_client_on_packet(gbnet_client_t* client, void (*on_packet)(gbnet_client_t* client, gbnet_packet_t* packet));
void		gbnet_client_on_disconnect(gbnet_client_t* client, void (*on_disconnect)(gbnet_client_t* client));
int		gbnet_client_poll(gbnet_client_t* client);
void		gbnet_client_destroy(gbnet_client_t* client);

gbnet_server_t* gbnet_server(int port);
int		gbnet_server_clients(gbnet_server_t* server);
void		gbnet_server_loop(gbnet_server_t* server);
void		gbnet_server_on_client(gbnet_server_t* server, void (*on_client)(gbnet_server_t* server, gbnet_client_t* client));
void		gbnet_server_on_tick(gbnet_server_t* server, void (*on_tick)(gbnet_server_t* server));
void		gbnet_server_broadcast(gbnet_server_t* server, gbnet_packet_t* packet);
void		gbnet_server_destroy(gbnet_server_t* server);

int gbnet_fd_send(int fd, const void* buf, int length);
int gbnet_fd_recv(int fd, void* buf, int length, int to);
int gbnet_packet_send(int fd, gbnet_packet_t* packet);
int gbnet_packet_recv(int fd, gbnet_packet_t* packet);

#endif
