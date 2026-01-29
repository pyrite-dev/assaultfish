#include <GearSrc/Net.h>

#include <GearSrc/Endian.h>
#include <GearSrc/Log.h>

#include <stb_ds.h>

enum states {
	Acknowledged = 0,
	WaitingAcknowledge
};

void GSNetStateRead(GSNetState* state, GSNetSocket sock, GSNetPacket* packet, GSNetAddress* address) {
	if(packet->header.flag & GSNetPacketFlagAcknowledge) {
		if(arrlen(state->tx) > 0 && state->txstate == WaitingAcknowledge && state->txindex == packet->header.index && state->txseq == packet->header.seq) {
			GSLog(state->engine, GSLogDebug, "%s: Received acknowledge packet %d:%d", state->name, state->txindex, state->txseq);

			state->txstate = Acknowledged;
			state->txseq++;
			if(state->txseq == (state->txtotal + 1)) {
				state->txseq = 0;
				state->txindex++;

				arrdel(state->tx, 0);
			}
		}
	} else {
		GSNetPacket pkt;
		if(state->rxindex == packet->header.index && state->rxseq == packet->header.seq) {
			GSLog(state->engine, GSLogDebug, "%s: Received packet %d:%d", state->name, state->rxindex, state->rxseq);

			if(state->rxseq == 0) {
				state->rxtotal = GSEndianSwapU32BE(*(GSU32*)packet->data);
			}

			state->rxseq++;
			if(state->rxseq == (state->rxtotal + 1)) {
				state->rxseq = 0;
				state->rxindex++;
			}
		}

		/* this is to prevent netcode from blowing up when
		 * acknowledge packet goes missing...
		 */
		pkt.header.flag |= GSNetPacketFlagAcknowledge;
		pkt.header.index = packet->header.index;
		pkt.header.seq	 = packet->header.seq;
		pkt.size	 = 0;

		GSNetPacketWrite(sock, &pkt, address);
	}
}

void GSNetStateWrite(GSNetState* state, GSNetSocket sock, GSNetAddress* address) {
	if(arrlen(state->tx) > 0) {
		if(state->txstate == Acknowledged) {
			int	    n = state->tx[0].size;
			GSNetPacket pkt;

			if(state->txseq == 0) {
				GSU32 t;

				state->txtotal = 0;

				while(n > 0) {
					n -= 508 - sizeof(pkt.header);
					state->txtotal++;
				}

				t = GSEndianSwapU32BE(state->txtotal);

				memcpy(pkt.data, &t, 4);
				pkt.size = 4;
			} else {
				int m = 508 - sizeof(pkt.header);
				int n = m * (state->txseq - 1);

				pkt.size = (n > m) ? m : n;
				memcpy(pkt.data, ((unsigned char*)state->tx[0].data) + n, pkt.size);
			}
			pkt.header.flag	 = 0;
			pkt.header.index = state->txindex;
			pkt.header.seq	 = state->txseq;

			GSNetPacketWrite(sock, &pkt, address);

			state->txstate = WaitingAcknowledge;
			if(state->engine->param->get_tick != NULL) state->txtick = state->engine->param->get_tick();

			GSLog(state->engine, GSLogDebug, "%s: Sent packet %d:%d", state->name, state->txindex, state->txseq);
		} else if(state->engine->param->get_tick == NULL) {
		} else if((state->engine->param->get_tick() - state->txtick) >= GSNetAcknowledgeTimeout) {
			GSLog(state->engine, GSLogDebug, "%s: Lost packet %d:%d", state->name, state->txindex, state->txseq);

			state->txstate = Acknowledged;
		}
	}
}
