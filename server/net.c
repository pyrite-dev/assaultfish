#include <af_common.h>

#include <stb_ds.h>

static fishsoup_server_t* server;

typedef struct opaque {
	char username[256 + 1];
} opaque_t;

static void on_packet(fishsoup_client_t* self, fishsoup_packet_t* pkt) {
	opaque_t* o = self->user;

	if(pkt->header.type == AF_PACKET_JOIN && pkt->header.length == sizeof(af_packet_join_t)) {
		af_packet_join_t* p_join = pkt->data;
		int		  i;

		o->username[256] = 0;
		memcpy(o->username, p_join->username, 256);

		af_common_log_info("Client %d: JOIN packet: Username \"%s\"\n", self->fd, o->username);

		for(i = 0; i < arrlen(server->clients); i++) {
			if(self->fd != server->clients[i].fd && strcmp(o->username, ((opaque_t*)server->clients[i].user)->username) == 0) {
				break;
			}
		}

		if(i == arrlen(server->clients)) {
			fishsoup_packet_t p;
			af_packet_ok_t	  p_ok;

			p.header.type	= AF_PACKET_OK;
			p.header.length = sizeof(p_ok);
			p.data		= &p_ok;

			memset(p_ok.reason, 0, 256);

			fishsoup_packet_send(self->fd, &p);

			af_common_log_info("Client %d: JOIN packet: Username is unused\n", self->fd);
		} else {
			fishsoup_packet_t p;
			af_packet_error_t p_error;

			p.header.type	= AF_PACKET_ERROR;
			p.header.length = sizeof(p_error);
			p.data		= &p_error;

			memset(p_error.reason, 0, 256);
			strcpy(p_error.reason, "Username is already used");

			fishsoup_packet_send(self->fd, &p);

			af_common_log_info("Client %d: JOIN packet: Username is already used\n", self->fd);
			memset(o->username, 0, 256 + 1);
		}
	}
}

static void on_disconnect(fishsoup_client_t* self) {
	free(self->user);
	af_common_log_info("Client %d: Disconnected\n", self->fd);
}

static void on_client(fishsoup_server_t* self, fishsoup_client_t* client) {
	opaque_t*	  o;
	fishsoup_packet_t pkt;
	af_packet_hello_t hello;

	af_common_log_info("Client %d: Connected\n", client->fd);

	o = client->user = malloc(sizeof(opaque_t));

	o->username[0] = 0;

	fishsoup_client_on_disconnect(client, on_disconnect);
	fishsoup_client_on_packet(client, on_packet);

	pkt.header.type	  = AF_PACKET_HELLO;
	pkt.header.length = sizeof(hello);
	pkt.data	  = &hello;

	af_common_version(&hello.major, &hello.minor, &hello.patch);

	fishsoup_packet_send(client->fd, &pkt);
}

static struct timespec c;
static void	       on_tick(fishsoup_server_t* self) {
	   struct timespec n;
	   unsigned long   t1, t2, d;

	   clock_gettime(CLOCK_MONOTONIC, &n);

	   t1 = n.tv_sec * 1000 + n.tv_nsec / 1000000;
	   t2 = c.tv_sec * 1000 + c.tv_nsec / 1000000;

	   d = t1 - t2; /* elapsed time */
	   af_common_log_info("Tick: %dms\n", d);

	   c = n;
}

void net_start(int port) {
	if(port == 0) port = AF_DEFAULT_PORT;

	if((server = fishsoup_server(port)) == NULL) return;

	fishsoup_server_on_client(server, on_client);
	fishsoup_server_on_tick(server, on_tick);
}

void net_loop(void) {
	if(server == NULL) return;

	clock_gettime(CLOCK_MONOTONIC, &c);
	fishsoup_server_loop(server);
}
