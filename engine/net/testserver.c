#include <gbnet.h>

void on(gbnet_server_t* server, gbnet_client_t* client) {
	gbnet_packet_t pkt;

	pkt.data	  = "Hello world!";
	pkt.header.length = strlen(pkt.data) + 1;
	pkt.header.flag	  = 0;
	pkt.header.type	  = NET_PACKET_USER;

	gbnet_packet_send(client->fd, &pkt);
}

int main() {
	gbnet_server_t* server = gbnet_server(0);
	gbnet_server_on_client(server, on);
	gbnet_server_loop(server);
}
