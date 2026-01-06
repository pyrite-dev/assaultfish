#include <gbnet.h>

void on(gbnet_client_t* client, gbnet_packet_t* pkt) {
	if(pkt->header.type == NET_PACKET_USER) printf("Received: %s\n", pkt->data);
}

int main() {
	gbnet_client_t* client = gbnet_client("127.0.0.1", 0);
	gbnet_client_on_packet(client, on);
	while(gbnet_client_poll(client) >= 0);
}
