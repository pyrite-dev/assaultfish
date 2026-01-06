#ifndef __FISHSOUP_H__
#define __FISHSOUP_H__

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
#define FISHSOUP_PORT 3219

#define FISHSOUP_PING 3

enum fishsoup_packet_type {
	FISHSOUP_PACKET_PING = 0,
	FISHSOUP_PACKET_PONG,
	FISHSOUP_PACKET_USER
};

typedef struct fishsoup_client	      fishsoup_client_t;
typedef struct fishsoup_server	      fishsoup_server_t;
typedef struct fishsoup_packet_header fishsoup_packet_header_t;
typedef struct fishsoup_packet	      fishsoup_packet_t;

struct fishsoup_client {
	int    fd;
	time_t last_ping;
	void (*on_packet)(fishsoup_client_t* client, fishsoup_packet_t* packet);
	void (*on_disconnect)(fishsoup_client_t* client);
	void* user;
	struct {
		int sent_ping;
		int cleanup;
	} server;
};

struct fishsoup_server {
	int		   fd;
	struct sockaddr_in address;
	fishsoup_client_t* clients;
	void*		   user;
	void (*on_client)(fishsoup_server_t* server, fishsoup_client_t* client);
	void (*on_tick)(fishsoup_server_t* server);
};

#pragma pack(1)
struct fishsoup_packet_header {
	unsigned char  type;
	unsigned short length; /* ok, seriously, who sends 64k at ONCE */
};
#pragma pack()

/* !!! REMEMBER TO FREE DATA AFTER RECV UNLESS IT'S NULL !!! */
struct fishsoup_packet {
	fishsoup_packet_header_t header;
	void*			 data;
};

fishsoup_client_t* fishsoup_client(const char* host, int port);
void		   fishsoup_client_on_packet(fishsoup_client_t* client, void (*on_packet)(fishsoup_client_t* client, fishsoup_packet_t* packet));
void		   fishsoup_client_on_disconnect(fishsoup_client_t* client, void (*on_disconnect)(fishsoup_client_t* client));
int		   fishsoup_client_poll(fishsoup_client_t* client);
void		   fishsoup_client_destroy(fishsoup_client_t* client);

fishsoup_server_t* fishsoup_server(int port);
int		   fishsoup_server_clients(fishsoup_server_t* server);
void		   fishsoup_server_loop(fishsoup_server_t* server);
void		   fishsoup_server_on_client(fishsoup_server_t* server, void (*on_client)(fishsoup_server_t* server, fishsoup_client_t* client));
void		   fishsoup_server_on_tick(fishsoup_server_t* server, void (*on_tick)(fishsoup_server_t* server));
void		   fishsoup_server_broadcast(fishsoup_server_t* server, fishsoup_packet_t* packet);
void		   fishsoup_server_destroy(fishsoup_server_t* server);

int fishsoup_fd_send(int fd, const void* buf, int length);
int fishsoup_fd_recv(int fd, void* buf, int length, int to);
int fishsoup_packet_send(int fd, fishsoup_packet_t* packet);
int fishsoup_packet_recv(int fd, fishsoup_packet_t* packet);

#endif
