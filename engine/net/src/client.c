#include <fishsoup.h>

fishsoup_client_t* fishsoup_client(const char* host, int port) {
	fishsoup_client_t* client = malloc(sizeof(*client));
	struct addrinfo	   hints;
	char		   p[32];
	struct addrinfo*   result;
	struct addrinfo*   rp;

	if(port == 0) port = FISHSOUP_PORT;
	sprintf(p, "%d", port);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family	  = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if(getaddrinfo(host, p, &hints, &result) != 0) {
		free(client);
		return NULL;
	}

	for(rp = result; rp != NULL; rp = rp->ai_next) {
		int val = 1;

		if((client->fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) < 0) {
			continue;
		}

		val = 1;
		setsockopt(client->fd, IPPROTO_TCP, TCP_NODELAY, (char*)&val, sizeof(val));

		if(connect(client->fd, rp->ai_addr, rp->ai_addrlen) >= 0) {
			break;
		}

		close(client->fd);
	}

	if(rp == NULL) {
		free(client);
		return NULL;
	}

	client->on_packet     = NULL;
	client->on_disconnect = NULL;

	return client;
}

void fishsoup_client_on_packet(fishsoup_client_t* client, void (*on_packet)(fishsoup_client_t* client, fishsoup_packet_t* packet)) {
	client->on_packet = on_packet;
}

void fishsoup_client_on_disconnect(fishsoup_client_t* client, void (*on_disconnect)(fishsoup_client_t* client)) {
	client->on_disconnect = on_disconnect;
}

int fishsoup_client_poll(fishsoup_client_t* client) {
	struct pollfd	  pfd;
	fishsoup_packet_t pkt;
	int		  ret;

	pfd.fd	   = client->fd;
	pfd.events = POLLIN | POLLPRI;
	ret	   = poll(&pfd, 1, 0);
	if(ret <= 0) return ret;

	ret = fishsoup_packet_recv(client->fd, &pkt);
	if(ret == 0) return -1; /* connection broke, probably */

	if(pkt.header.type == FISHSOUP_PACKET_PING) {
		fishsoup_packet_t ret;
		client->last_ping = time(NULL);

		ret.header.type	  = FISHSOUP_PACKET_PONG;
		ret.header.length = 0;
		ret.data	  = NULL;
		if(fishsoup_packet_send(client->fd, &ret) == 0) {
			if(pkt.data != NULL) free(pkt.data);
			return -1;
		}
	}
	if(client->on_packet != NULL) client->on_packet(client, &pkt);

	if(pkt.data != NULL) free(pkt.data);

	return 1;
}

void fishsoup_client_destroy(fishsoup_client_t* client) {
	if(client->on_disconnect != NULL) client->on_disconnect(client);
	close(client->fd);
	free(client);
}
