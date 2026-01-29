#include <GearSrc/Net.h>

#include <GearSrc/Log.h>

GSNetClient GSNetClientOpen(GSClient client, const char* hostname, int port) {
	GSNetClient net = malloc(sizeof(*net));

	memset(net, 0, sizeof(*net));

	net->engine = client->engine;

	if((net->fd = GSNetBaseClient(hostname, port)) < 0) {
		GSLog(client->engine, GSLogError, "Failed to initialize socket");
		GSNetClientClose(net);

		return NULL;
	}

	return net;
}

void GSNetClientClose(GSNetClient client) {
	if(client->fd >= 0) GSNetBaseClose(client->fd);

	free(client);
}
