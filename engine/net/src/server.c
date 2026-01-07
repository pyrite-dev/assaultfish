#include <gbnet.h>

#include "stb_ds.h"

gbnet_server_t* gbnet_server(int port) {
	gbnet_server_t* server = malloc(sizeof(*server));
	int		val;

	if(port == 0) port = NET_PORT;

	if((server->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		free(server);
		return NULL;
	}

	val = 1;
	setsockopt(server->fd, IPPROTO_TCP, TCP_NODELAY, (char*)&val, sizeof(val));

	server->address.sin_family	= AF_INET;
	server->address.sin_addr.s_addr = INADDR_ANY;
	server->address.sin_port	= htons(port);

	if(bind(server->fd, (struct sockaddr*)&server->address, sizeof(server->address)) < 0) {
		close(server->fd);
		free(server);
		return NULL;
	}

	if(listen(server->fd, 128) < 0) {
		close(server->fd);
		free(server);
		return NULL;
	}

	server->clients	  = NULL;
	server->on_client = NULL;
	server->on_tick	  = NULL;

	return server;
}

int gbnet_server_clients(gbnet_server_t* server) {
	return arrlen(server->clients);
}

void gbnet_server_loop(gbnet_server_t* server) {
	struct pollfd* pfd;

	while(1) {
		int i;
		int ret;
		pfd = malloc(sizeof(*pfd) * (1 + arrlen(server->clients)));

		pfd[0].fd     = server->fd;
		pfd[0].events = POLLIN | POLLPRI;

		for(i = 0; i < arrlen(server->clients); i++) {
			pfd[1 + i].fd	  = server->clients[i].fd;
			pfd[1 + i].events = POLLIN | POLLPRI;
		}

		ret = poll(pfd, 1 + arrlen(server->clients), 50);
		if(ret < 0) {
			free(pfd);
			break;
		}

		if(server->on_tick != NULL) server->on_tick(server);

		if(pfd[0].revents & POLLIN) {
			gbnet_client_t	   client;
			struct sockaddr_in address;
			int		   len = sizeof(address);

			client.fd		= accept(server->fd, (struct sockaddr*)&address, &len);
			client.last_ping	= time(NULL);
			client.server.cleanup	= 0;
			client.server.sent_ping = 0;
			client.on_packet	= NULL;

			arrput(server->clients, client);

			if(server->on_client != NULL) server->on_client(server, &server->clients[arrlen(server->clients) - 1]);

#ifdef DEBUG
			fprintf(stderr, "Client %d accepted\n", server->clients[i].fd);
#endif
		}

		for(i = 0; i < arrlen(server->clients); i++) {
			time_t diff;
			if(server->clients[i].server.cleanup) continue;
			if(pfd[1 + i].revents & POLLIN) {
				gbnet_packet_t pkt;

				if(gbnet_packet_recv(server->clients[i].fd, &pkt) == 0) {
					server->clients[i].server.cleanup = 1;
				} else {
					if(pkt.header.type == NET_PACKET_PONG && server->clients[i].server.sent_ping) {
						server->clients[i].last_ping	    = time(NULL);
						server->clients[i].server.sent_ping = 0;

#ifdef DEBUG
						fprintf(stderr, "Pong from client %d\n", server->clients[i].fd);
#endif
					} else if(pkt.header.type == NET_PACKET_PING) {
						gbnet_packet_t ret;

						ret.header.type	  = NET_PACKET_PONG;
						ret.header.length = 0;
						ret.data	  = NULL;

						if(gbnet_packet_send(server->clients[i].fd, &ret) == 0) {
							server->clients[i].server.cleanup = 1;
						} else {
#ifdef DEBUG
							fprintf(stderr, "Ping from client %d, sending back pong\n", server->clients[i].fd);
#endif
						}
					}

					if(!server->clients[i].server.cleanup && server->clients[i].on_packet != NULL) server->clients[i].on_packet(&server->clients[i], &pkt);

					if(pkt.data != NULL) free(pkt.data);
				}
			}
			if(server->clients[i].server.cleanup) continue;

			diff = time(NULL) - server->clients[i].last_ping;

			if(diff == NET_PING && !server->clients[i].server.sent_ping) {
				gbnet_packet_t pkt;

				pkt.header.type	  = NET_PACKET_PING;
				pkt.header.length = 0;
				pkt.data	  = NULL;

				if(gbnet_packet_send(server->clients[i].fd, &pkt) == 0) {
					server->clients[i].server.cleanup = 1;
#ifdef DEBUG
					fprintf(stderr, "Failed to ping client %d\n", server->clients[i].fd);
#endif
				} else {
					server->clients[i].server.sent_ping = 1;
#ifdef DEBUG
					fprintf(stderr, "Pinging client %d\n", server->clients[i].fd);
#endif
				}
			} else if(diff == NET_PING * 2 && server->clients[i].server.sent_ping) {
				server->clients[i].server.cleanup = 1;
#ifdef DEBUG
				fprintf(stderr, "Ping timeout (client %d)\n", server->clients[i].fd);
#endif
			}
		}

		for(i = 0; i < arrlen(server->clients); i++) {
			if(server->clients[i].server.cleanup) {
#ifdef DEBUG
				fprintf(stderr, "Client %d cleanup\n", server->clients[i].fd);
#endif
				if(server->clients[i].on_disconnect != NULL) server->clients[i].on_disconnect(&server->clients[i]);
				close(server->clients[i].fd);
				arrdel(server->clients, i);
				i--;
			}
		}

		free(pfd);
	}
}

void gbnet_server_on_client(gbnet_server_t* server, void (*on_client)(gbnet_server_t* server, gbnet_client_t* client)) {
	server->on_client = on_client;
}

void gbnet_server_on_tick(gbnet_server_t* server, void (*on_tick)(gbnet_server_t* server)) {
	server->on_tick = on_tick;
}

void gbnet_server_destroy(gbnet_server_t* server) {
	int i;
	for(i = 0; i < arrlen(server->clients); i++) {
		if(server->clients[i].on_disconnect != NULL) server->clients[i].on_disconnect(&server->clients[i]);
		close(server->clients[i].fd);
	}
	arrfree(server->clients);
	close(server->fd);
	free(server);
}

void gbnet_server_broadcast(gbnet_server_t* server, gbnet_packet_t* packet) {
	int i;
	for(i = 0; i < arrlen(server->clients); i++) {
		if(gbnet_packet_send(server->clients[i].fd, packet) == 0) {
			server->clients[i].server.cleanup = 1;
		}
	}
}
