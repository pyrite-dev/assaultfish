#ifndef __GEARSRC_NET_H__
#define __GEARSRC_NET_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GSNetDefaultPort 23903

#define GSNetPacketFlagAcknowledge (1 << 0)

/* net_client.c */
GSDECL GSNetClient GSNetClientOpen(GSClient net, const char* hostname, int port);
GSDECL void	   GSNetClientStep(GSNetClient net);
GSDECL void	   GSNetClientClose(GSNetClient net);

/* net_server.c */
GSDECL GSNetServer GSNetServerOpen(GSServer net, int port);
GSDECL void	   GSNetServerStep(GSNetServer net);
GSDECL void	   GSNetServerClose(GSNetServer net);

/* net_base.c */
GSDECL GSBool GSNetBaseHasData(int fd);
GSDECL int    GSNetBaseServer(int port);
GSDECL int    GSNetBaseClient(const char* hostname, int port);
GSDECL void   GSNetBaseClose(int fd);
GSDECL int    GSNetBaseRead(int fd, void* data, int size, GSNetAddress* address); /* always read 508 bytes! */
GSDECL void   GSNetBaseWrite(int fd, void* data, int size, GSNetAddress* address);

/* net_packet.c */
GSDECL void GSNetPacketRead(int fd, GSNetPacket* packet, GSNetAddress* address);
GSDECL void GSNetPacketWrite(int fd, GSNetPacket* packet, GSNetAddress* address);

/* net_state.c */
GSDECL void GSNetStateRead(GSNetState* state, int fd, GSNetPacket* packet, GSNetAddress* address);
GSDECL void GSNetStateWrite(GSNetState* state, int fd, GSNetAddress* address);

#ifdef __cplusplus
}
#endif

#endif
