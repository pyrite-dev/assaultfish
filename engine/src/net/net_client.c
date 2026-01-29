#include <GearSrc/Net.h>

#include <GearSrc/Log.h>
#include <GearSrc/Endian.h>

#include <stb_ds.h>

GSNetClient GSNetClientOpen(GSClient client, const char* hostname, int port) {
	GSNetClient net = malloc(sizeof(*net));

	memset(net, 0, sizeof(*net));

	net->engine = client->engine;

	if((net->fd = GSNetBaseClient(hostname, port)) < 0) {
		GSLog(client->engine, GSLogError, "Failed to initialize socket");
		GSNetClientClose(net);

		return NULL;
	}

	GSBinary b;
	b.data = strdup("Hello");
	b.size = 5;
	arrput(net->tx, b);

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
			if(arrlen(net->tx) > 0 && net->txstate == WaitingAcknowledge && net->txindex == pkt.header.index && net->txseq == pkt.header.seq) {
				GSLog(net->engine, GSLogDebug, "Received ACK packet %d:%d", net->txindex, net->txseq);

				net->txstate = Acknowledged;
				net->txseq++;
				if(net->txseq == (net->txtotal + 1)){
					net->txseq = 0;
					net->txindex++;

					arrdel(net->tx, 0);
				}
			}
		} else {
			if(net->rxindex == pkt.header.index && net->rxseq == pkt.header.seq) {
				GSLog(net->engine, GSLogDebug, "Received packet %d:%d", net->rxindex, net->rxseq);

				pkt.header.flag |= GSNetPacketFlagAcknowledge;
				pkt.header.index = net->rxindex;
				pkt.header.seq	 = net->rxseq;
				pkt.size	 = 0;

				if(net->rxseq == 0){
					net->rxtotal = GSEndianSwapU32BE(*(GSU32*)pkt.data);
				}

				GSNetPacketWrite(net->fd, &pkt, &addr);

				net->rxseq++;
				if(net->rxseq == (net->rxtotal + 1)){
					net->rxseq = 0;
					net->rxindex++;
				}
			}
		}
	}

	if(arrlen(net->tx) > 0){
		if(net->txstate == Acknowledged){
			int n = net->tx[0].size;

			if(net->txseq == 0){
				net->txtotal = 0;

				while(n > 0){
					n -= 508 - sizeof(pkt.header);
					net->txtotal++;
				}

			}

			net->txstate = WaitingAcknowledge;
			if(net->engine->param->get_tick != NULL) net->txtick = net->engine->param->get_tick();

			GSLog(net->engine, GSLogDebug, "Sent packet %d:%d", net->txindex, net->txseq);
		}else if(net->engine->param->get_tick == NULL){
		}else if((net->engine->param->get_tick() - net->txtick) >= 500){
			GSLog(net->engine, GSLogDebug, "Lost pcaket %d:%d", net->txindex, net->txseq);

			net->txstate = Acknowledged;
		}
	}
}

void GSNetClientClose(GSNetClient net) {
	if(net->fd >= 0) GSNetBaseClose(net->fd);

	free(net);
}
