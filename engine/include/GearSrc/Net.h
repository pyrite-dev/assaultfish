#ifndef __GEARSRC_NET_H__
#define __GEARSRC_NET_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  6  bits reserved
 *  1  bit  unsafe or safe
 *  1  bit  ack or normal
 *  4 bytes index
 *  4 bytes sequence
 *
 * if unsafe:
 *  n bytes data
 * else if ack:
 *   no data continues
 * else if sequence is 0:
 *  4 bytes sequence count
 * else:
 *  n bytes data
 */

#define GSNetDefaultPort 23903
#define GSNetAcknowledgeTimeout 500

#define GSNetPacketFlagAcknowledge (1 << 0)
#define GSNetPacketFlagUnsafe (1 << 1)

/* net_client.c */
GSDECL GSNetClient GSNetClientOpen(GSClient net, const char* hostname, int port);
GSDECL void	   GSNetClientStep(GSNetClient net);
GSDECL void	   GSNetClientClose(GSNetClient net);

/* net_server.c */
GSDECL GSNetServer GSNetServerOpen(GSServer net, int port);
GSDECL void	   GSNetServerStep(GSNetServer net);
GSDECL void	   GSNetServerClose(GSNetServer net);

/* net_base.c */
GSDECL GSBool	   GSNetBaseHasData(GSNetSocket sock);
GSDECL GSNetSocket GSNetBaseServer(int port);
GSDECL GSNetSocket GSNetBaseClient(const char* hostname, int port, GSNetAddress* address);
GSDECL void	   GSNetBaseClose(GSNetSocket sock);
GSDECL int	   GSNetBaseRead(GSNetSocket sock, void* data, int size, GSNetAddress* address); /* always read 508 bytes! */
GSDECL void	   GSNetBaseWrite(GSNetSocket sock, void* data, int size, GSNetAddress* address);

/* net_packet.c */
GSDECL void GSNetPacketRead(GSNetSocket sock, GSNetPacket* packet, GSNetAddress* address);
GSDECL void GSNetPacketWrite(GSNetSocket sock, GSNetPacket* packet, GSNetAddress* address);

/* net_state.c */
GSDECL void   GSNetStateCheckRead(GSNetState* state, GSNetSocket sock, GSNetPacket* packet, GSNetAddress* address);
GSDECL void   GSNetStateCheckWrite(GSNetState* state, GSNetSocket sock, GSNetAddress* address);
GSDECL void   GSNetStateInit(GSNetState* state, GSEngine engine);
GSDECL void   GSNetStateDeinit(GSNetState* state);
GSDECL GSBool GSNetStateRead(GSNetState* state, void** data, int* size); /* remember to free what gets returned in data! */
GSDECL void   GSNetStateWrite(GSNetState* state, void* data, int size);
GSDECL void   GSNetStateWriteUnsafe(GSNetState* state, void* data, int size);

#ifdef __cplusplus
}
#endif

#endif
