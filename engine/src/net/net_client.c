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

enum states {
	Acknowledged = 0,
	WaitingAcknowledge
};

void GSNetClientStep(GSNetClient net) {
	GSNetPacket  pkt;
	GSNetAddress addr;

	while(GSNetBaseHasData(net->fd)) {
		GSNetPacketRead(net->fd, &pkt, &addr);

		if(pkt.header.flag & GSNetPacketFlagAcknowledge) {
			if(net->txstate == WaitingAcknowledge && net->txindex == pkt.header.index && net->txseq == pkt.header.seq) {
				GSLog(net->engine, GSLogDebug, "ACK received for %d:%d", net->txindex, net->txseq);

				if(net->txseq == net->txtotal) net->txstate = Acknowledged;
			}
		} else {
			if(net->rxstate == Acknowledged && net->rxindex == pkt.header.index && net->rxseq == pkt.header.seq) {
				pkt.header.flag |= GSNetPacketFlagAcknowledge;
				pkt.header.index = net->rxindex;
				pkt.header.seq	 = net->rxseq;
				pkt.size	 = 0;

				GSNetPacketWrite(net->fd, &pkt, &addr);
			}
		}
	}
}

void GSNetClientClose(GSNetClient net) {
	if(net->fd >= 0) GSNetBaseClose(net->fd);

	free(net);
}
