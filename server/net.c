#include <af_common.h>

static fishsoup_server_t* server;

typedef struct opaque {
	char username[256 + 1];
} opaque_t;

static void on_packet(fishsoup_client_t* self, fishsoup_packet_t* pkt) {
	opaque_t* o = self->user;

	if(pkt->header.type == AF_PACKET_JOIN && pkt->header.length == sizeof(af_packet_join_t)) {
		af_packet_join_t* p_join = pkt->data;

		o->username[256] = 0;
		memcpy(o->username, p_join->username, 256);

		af_common_log_info("Client %d: JOIN packet: Username \"%s\"\n", self->fd, o->username);
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

void net_start(int port) {
	if(port == 0) port = AF_DEFAULT_PORT;

	if((server = fishsoup_server(port)) == NULL) return;

	fishsoup_server_on_client(server, on_client);
}

void net_loop(void) {
	if(server == NULL) return;

	fishsoup_server_loop(server);
}
