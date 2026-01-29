#ifndef __GEARSRC_NET_H__
#define __GEARSRC_NET_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/* net_client.c */
GSDECL GSNetClient GSNetClientOpen(GSClient client, const char* hostname, int port);
GSDECL void	   GSNetClientClose(GSNetClient client);

/* net_server.c */
GSDECL GSNetServer GSNetServerOpen(GSServer server, int port);
GSDECL void	   GSNetServerClose(GSNetServer server);

/* net_base.c */
GSDECL GSBool GSNetBaseHasData(int fd);
GSDECL int    GSNetBaseServer(int port);
GSDECL int    GSNetBaseClient(const char* hostname, int port);
GSDECL void   GSNetBaseClose(int fd);
GSDECL void   GSNetBaseRead(int fd, void* data, int size, GSNetAddress* address); /* always read 508 bytes! */
GSDECL void   GSNetBaseWrite(int fd, void* data, int size, GSNetAddress* address);

/* net_packet.c */
GSDECL void GSNetPacketRead(int fd, GSNetPacket* packet, GSNetAddress* address);
GSDECL void GSNetPacketWrite(int fd, GSNetPacket* packet, GSNetAddress* address);

#ifdef __cplusplus
}
#endif

#endif
