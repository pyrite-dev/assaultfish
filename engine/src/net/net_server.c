#include <GearSrc/Net.h>

#include <GearSrc/Log.h>

#include <stb_ds.h>

GSNetServer GSNetServerOpen(GSServer server, int port) {
	GSNetServer net = malloc(sizeof(*net));

	memset(net, 0, sizeof(*net));

	net->engine = server->engine;

	if((net->fd = GSNetBaseServer(port)) < 0) {
		GSLog(server->engine, GSLogError, "Failed to initialize socket");
		GSNetServerClose(net);

		return NULL;
	}

	return net;
}

void GSNetServerStep(GSNetServer net) {
	int i;
	GSNetPacket pkt;
	GSNetAddress addr;

	while(GSNetBaseHasData(net->fd)){
		GSNetState v;

		memset(&v, 0, sizeof(v));
		v.engine = net->engine;

		GSNetPacketRead(net->fd, &pkt, &addr);

		i = hmgeti(net->client, addr);
		if(i != -1) v = net->client[i].value;

		GSNetStateRead(&v, net->fd, &pkt, &addr);
		hmput(net->client, addr, v);
	}

	for(i = 0; i < hmlen(net->client); i++){
		GSNetStateWrite(&net->client[i].value, net->fd, &net->client[i].key);
	}
}

void GSNetServerClose(GSNetServer net) {
	if(net->fd >= 0) GSNetBaseClose(net->fd);

	free(net);
}
