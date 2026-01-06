#include <fishsoup.h>

void on(fishsoup_client_t* client, fishsoup_packet_t* pkt) {
	if(pkt->header.type == FISHSOUP_PACKET_USER) printf("Received: %s\n", pkt->data);
}

int main() {
	fishsoup_client_t* client = fishsoup_client("127.0.0.1", 0);
	fishsoup_client_on_packet(client, on);
	while(fishsoup_client_poll(client) >= 0);
}
