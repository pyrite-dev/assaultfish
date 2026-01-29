#include <GearSrc/Net.h>

#include <GearSrc/Endian.h>
#include <GearSrc/Log.h>

#include <stb_ds.h>

enum states {
	Acknowledged = 0,
	WaitingAcknowledge
};

void GSNetStateRead(GSNetState* state, int fd, GSNetPacket* packet, GSNetAddress* address){
	if(packet->header.flag & GSNetPacketFlagAcknowledge) {
		if(arrlen(state->tx) > 0 && state->txstate == WaitingAcknowledge && state->txindex == packet->header.index && state->txseq == packet->header.seq) {
			GSLog(state->engine, GSLogDebug, "Received ACK packet %d:%d", state->txindex, state->txseq);

			state->txstate = Acknowledged;
			state->txseq++;
			if(state->txseq == (state->txtotal + 1)){
				state->txseq = 0;
				state->txindex++;

				arrdel(state->tx, 0);
			}
		}
	} else {
		if(state->rxindex == packet->header.index && state->rxseq == packet->header.seq) {
			GSNetPacket pkt;

			GSLog(state->engine, GSLogDebug, "Received packet %d:%d", state->rxindex, state->rxseq);

			pkt.header.flag |= GSNetPacketFlagAcknowledge;
			pkt.header.index = state->rxindex;
			pkt.header.seq	 = state->rxseq;
			pkt.size	 = 0;

			if(state->rxseq == 0){
				state->rxtotal = GSEndianSwapU32BE(*(GSU32*)pkt.data);
			}

			GSNetPacketWrite(fd, &pkt, address);

			state->rxseq++;
			if(state->rxseq == (state->rxtotal + 1)){
				state->rxseq = 0;
				state->rxindex++;
			}
		}
	}
}

void GSNetStateWrite(GSNetState* state, int fd, GSNetAddress* address){
	if(arrlen(state->tx) > 0){
		if(state->txstate == Acknowledged){
			int n = state->tx[0].size;
			GSNetPacket pkt;

			if(state->txseq == 0){
				GSU32 t;

				state->txtotal = 0;

				while(n > 0){
					n -= 508 - sizeof(pkt.header);
					state->txtotal++;
				}

				t = GSEndianSwapU32BE(state->txtotal);

				memcpy(pkt.data, &t, 4);
				pkt.size = 4;
			}else{
				int m = 508 - sizeof(pkt.header);
				int n = m * (state->txseq - 1);

				pkt.size = (n > m) ? m : n;
				memcpy(pkt.data, ((unsigned char*)state->tx[0].data) + n, pkt.size);
			}
			pkt.header.flag = 0;
			pkt.header.index = state->txindex;
			pkt.header.seq = state->txseq;

			GSNetPacketWrite(fd, &pkt, address);

			state->txstate = WaitingAcknowledge;
			if(state->engine->param->get_tick != NULL) state->txtick = state->engine->param->get_tick();

			GSLog(state->engine, GSLogDebug, "Sent packet %d:%d", state->txindex, state->txseq);
		}else if(state->engine->param->get_tick == NULL){
		}else if((state->engine->param->get_tick() - state->txtick) >= 500){
			GSLog(state->engine, GSLogDebug, "Lost packet %d:%d", state->txindex, state->txseq);

			state->txstate = Acknowledged;
		}
	}
}
