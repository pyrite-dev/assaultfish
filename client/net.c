#include <af_common.h>

static fishsoup_client_t* client = NULL;

enum OPAQUE_STATES {
	OPAQUE_WAITING_HELLO = 0,
	OPAQUE_SENT_JOIN
};

typedef struct opaque {
	char username[256];
	int  state;
} opaque_t;

static void net_return(MwWidget handle, void* user, void* client) {
	scene_change(AF_SCENE_MAIN);
}

static void on_packet(fishsoup_client_t* self, fishsoup_packet_t* pkt) {
	opaque_t* o = self->user;

	if(pkt->header.type == AF_PACKET_HELLO && pkt->header.length == sizeof(af_packet_hello_t) && o->state == OPAQUE_WAITING_HELLO) {
		af_packet_hello_t* p_hello = pkt->data;
		unsigned char	   mj, mi, pt;

		af_common_version(&mj, &mi, &pt);

		af_common_log_info("HELLO packet: Server version %hhu.%hhu.%hhu\n", p_hello->major, p_hello->minor, p_hello->patch);

		if(mj == p_hello->major && mi >= p_hello->minor && (mi == p_hello->minor ? (pt >= p_hello->patch) : (1))) {
			fishsoup_packet_t p;
			af_packet_join_t  p_join;

			af_common_log_info("Server version is compatible with this client\n");

			p.header.type	= AF_PACKET_JOIN;
			p.header.length = sizeof(p_join);
			p.data		= &p_join;

			memcpy(p_join.username, o->username, 256);

			fishsoup_packet_send(self->fd, &p);

			o->state = OPAQUE_SENT_JOIN;

			return;
		}

		af_common_log_info("Server version is incompatible with this client\n");
		MwAddUserHandler(ui_message("Server version is incompatible with this client"), MwNactivateHandler, net_return, NULL);
		fishsoup_client_destroy(self);
		client = NULL;
	}
}

static void on_disconnect(fishsoup_client_t* self) {
	free(self->user);
}

void net_connect(const char* username, const char* hostname, int port) {
	opaque_t* o;

	scene_change(AF_SCENE_CONNECTING);
	client = fishsoup_client(hostname, port == 0 ? AF_DEFAULT_PORT : port);
	if(client == NULL) {
		MwAddUserHandler(ui_message("Failed to connect"), MwNactivateHandler, net_return, NULL);
		return;
	}

	o = client->user = malloc(sizeof(opaque_t));
	o->state	 = OPAQUE_WAITING_HELLO;
	memset(o->username, 0, 256);
	memcpy(o->username, username, strlen(username));

	fishsoup_client_on_packet(client, on_packet);
	fishsoup_client_on_disconnect(client, on_disconnect);
}

void net_poll(void) {
	int ret;
	if(client == NULL) return;

	while(client != NULL && (ret = fishsoup_client_poll(client)) > 0);
	if(ret < 0) {
		MwAddUserHandler(ui_message("Connection destroyed"), MwNactivateHandler, net_return, NULL);
		fishsoup_client_destroy(client);
		client = NULL;
		return;
	}
}
