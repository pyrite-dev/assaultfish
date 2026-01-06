#include <fishsoup.h>

void on(fishsoup_server_t* server, fishsoup_client_t* client) {
	fishsoup_packet_t pkt;

	pkt.data	  = "Hello world!";
	pkt.header.length = strlen(pkt.data) + 1;
	pkt.header.flag	  = 0;
	pkt.header.type	  = FISHSOUP_PACKET_USER;

	fishsoup_packet_send(client->fd, &pkt);
}

int main() {
	fishsoup_server_t* server = fishsoup_server(0);
	fishsoup_server_on_client(server, on);
	fishsoup_server_loop(server);
}
