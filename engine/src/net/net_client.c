#include <GearSrc/Net.h>

#include <GearSrc/Log.h>
#include <GearSrc/Endian.h>

#include <stb_ds.h>

GSNetClient GSNetClientOpen(GSClient client, const char* hostname, int port) {
	GSNetClient net = malloc(sizeof(*net));

	memset(net, 0, sizeof(*net));

	net->engine = client->engine;

	if((net->sock = GSNetBaseClient(hostname, port, &net->address)) < 0) {
		GSLog(client->engine, GSLogError, "Failed to initialize socket");
		GSNetClientClose(net);

		return NULL;
	}

	GSNetStateInit(&net->state, client->engine);
	strcpy(net->state.name, "Client");

	return net;
}

void GSNetClientStep(GSNetClient net) {
	GSNetPacket pkt;

	do {
		while(GSNetBaseHasData(net->sock)) {
			GSNetPacketRead(net->sock, &pkt, &net->address);
			GSNetStateRead(&net->state, net->sock, &pkt, &net->address);
		}

		GSNetStateWrite(&net->state, net->sock, &net->address);
	} while(GSNetBaseHasData(net->sock));
}

void GSNetClientClose(GSNetClient net) {
	if(net->sock != NULL) {
		GSNetStateDeinit(&net->state);
		GSNetBaseClose(net->sock);
	}

	free(net);
}
