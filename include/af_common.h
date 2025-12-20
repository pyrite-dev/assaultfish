#ifndef __AF_COMMON_H__
#define __AF_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

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
	AF_PACKET_HELLO = FISHSOUP_PACKET_USER,
	AF_PACKET_OK,
	AF_PACKET_ERROR,
	AF_PACKET_JOIN
};

#pragma pack(1)
typedef struct af_packet_hello {
	unsigned char major;
	unsigned char minor;
	unsigned char patch;
} af_packet_hello_t;

typedef struct af_packet_ok {
	char reason[256];
} af_packet_ok_t;

typedef struct af_packet_error {
	char reason[256];
} af_packet_error_t;

typedef struct af_packet_join {
	char username[256];
} af_packet_join_t;
#pragma pack()

/* version.c */
void af_common_version(unsigned char* v_major, unsigned char* v_minor, unsigned char* v_patch);

/* log.c */
void af_common_log_info(const char* fmt, ...);

#endif
