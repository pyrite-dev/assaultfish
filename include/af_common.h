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

/* should be broadcasted! otherwise other user wouldn't know */
typedef struct af_pkt_join {
	unsigned char length;
	char*	      name;
} af_pkt_join_t;

#endif
