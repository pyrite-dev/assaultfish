#include <GearSrc/Net.h>

#include <GearSrc/Log.h>

#include <stb_ds.h>

GSNetServer GSNetServerOpen(GSServer server, int port) {
	GSNetServer net = malloc(sizeof(*net));

	memset(net, 0, sizeof(*net));

	net->engine = server->engine;

	if((net->sock = GSNetBaseServer(port)) < 0) {
		GSLog(server->engine, GSLogError, "Failed to initialize socket");
		GSNetServerClose(net);

		return NULL;
	}

	return net;
}

void GSNetServerStep(GSNetServer net) {
	int	     i;
	GSNetPacket  pkt;
	GSNetAddress addr;

	do {
		while(GSNetBaseHasData(net->sock)) {
			GSNetPacketRead(net->sock, &pkt, &addr);

			i = hmgeti(net->client, addr);
			if(i == -1) {
				GSNetState v;

				memset(&v, 0, sizeof(v));
				GSNetStateInit(&v, net->engine);
				strcpy(v.name, "Server");

				hmput(net->client, addr, v);

				i = hmgeti(net->client, addr);
			}

			GSNetStateRead(&net->client[i].value, net->sock, &pkt, &addr);
		}

		for(i = 0; i < hmlen(net->client); i++) {
			GSNetStateWrite(&net->client[i].value, net->sock, &net->client[i].key);
		}
	} while(GSNetBaseHasData(net->sock));
}

void GSNetServerClose(GSNetServer net) {
	if(net->sock >= 0) GSNetBaseClose(net->sock);

	free(net);
}
