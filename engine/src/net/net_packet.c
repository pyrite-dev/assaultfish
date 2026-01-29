#include <GearSrc/Net.h>

#include <GearSrc/Endian.h>

void GSNetPacketRead(int fd, GSNetPacket* packet, GSNetAddress* address) {
	unsigned char buf[508];

	packet->size = GSNetBaseRead(fd, buf, 508, address) - sizeof(packet->header);

	memcpy(&packet->header, buf, sizeof(packet->header));
	memcpy(packet->data, buf + sizeof(packet->header), packet->size);

	packet->header.index	= GSEndianSwapU32BE(packet->header.index);
	packet->header.sequence = GSEndianSwapU32BE(packet->header.sequence);
}

void GSNetPacketWrite(int fd, GSNetPacket* packet, GSNetAddress* address) {
	GSNetPacketHeader header = packet->header;
	unsigned char	  buf[508];

	header.index	= GSEndianSwapU32BE(header.index);
	header.sequence = GSEndianSwapU32BE(header.sequence);

	memcpy(buf, &header, sizeof(header));
	memcpy(buf + sizeof(header), packet->data, packet->size);

	GSNetBaseWrite(fd, buf, sizeof(header) + packet->size, address);
}
