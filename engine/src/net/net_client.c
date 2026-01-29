#include <GearSrc/Net.h>

#include <GearSrc/Log.h>
#include <GearSrc/Endian.h>

#include <stb_ds.h>

GSNetClient GSNetClientOpen(GSClient client, const char* hostname, int port) {
	GSNetClient net = malloc(sizeof(*net));

	memset(net, 0, sizeof(*net));

	net->engine = client->engine;

	if((net->fd = GSNetBaseClient(hostname, port, &net->address)) < 0) {
		GSLog(client->engine, GSLogError, "Failed to initialize socket");
		GSNetClientClose(net);

		return NULL;
	}

	net->state.engine = net->engine;

	GSBinary b;
	b.data = strdup("Hello");
	b.size = 5;
	arrput(net->state.tx, b);

	return net;
}

void GSNetClientStep(GSNetClient net) {
	GSNetPacket pkt;

	while(GSNetBaseHasData(net->fd)){
		GSNetPacketRead(net->fd, &pkt, &net->address);
		GSNetStateRead(&net->state, net->fd, &pkt, &net->address);
	}

	GSNetStateWrite(&net->state, net->fd, &net->address);
}

void GSNetClientClose(GSNetClient net) {
	if(net->fd >= 0) GSNetBaseClose(net->fd);

	free(net);
}
