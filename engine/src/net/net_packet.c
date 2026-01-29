#include <GearSrc/Net.h>

#include <GearSrc/Endian.h>

void GSNetPacketRead(GSNetSocket sock, GSNetPacket* packet, GSNetAddress* address) {
	unsigned char buf[508];

	packet->size = GSNetBaseRead(sock, buf, 508, address) - sizeof(packet->header);

	memcpy(&packet->header, buf, sizeof(packet->header));
	memcpy(packet->data, buf + sizeof(packet->header), packet->size);

	packet->header.index = GSEndianSwapU32BE(packet->header.index);
	packet->header.seq   = GSEndianSwapU32BE(packet->header.seq);
}

void GSNetPacketWrite(GSNetSocket sock, GSNetPacket* packet, GSNetAddress* address) {
	GSNetPacketHeader header = packet->header;
	unsigned char	  buf[508];

	header.index = GSEndianSwapU32BE(header.index);
	header.seq   = GSEndianSwapU32BE(header.seq);

	memcpy(buf, &header, sizeof(header));
	memcpy(buf + sizeof(header), packet->data, packet->size);

	GSNetBaseWrite(sock, buf, sizeof(header) + packet->size, address);
}
