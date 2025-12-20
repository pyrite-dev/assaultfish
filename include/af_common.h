#ifndef __AF_COMMON_H__
#define __AF_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fishsoup.h>

#include <af_config.h>

#ifdef AF_SERVER
#include <af_server.h>
#endif

#ifdef AF_CLIENT
#include <af_client.h>
#endif

#define AF_DEFAULT_PORT 5312
#define AF_VERSION "0.0.0"
#define AF_COPYRIGHT "Copyright (C) 2025 Fishsoup"

enum AF_USER_PACKETS {
	AF_PACKET_JOIN = FISHSOUP_PACKET_USER
};

#pragma pack(1)
typedef struct af_packet_join {
	char name[256];
} af_packet_join_t;
#pragma pack()

#endif
