#include <GearSrc/Net.h>

#include <GearSrc/Log.h>

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
}

void GSNetServerClose(GSNetServer net) {
	if(net->fd >= 0) GSNetBaseClose(net->fd);

	free(net);
}
