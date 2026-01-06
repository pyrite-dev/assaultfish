#include <gbnet.h>

#include <zlib.h>

#define CHUNK 512

int gbnet_packet_send(int fd, gbnet_packet_t* packet) {
	int			 r;
	gbnet_packet_header_t h;
	int			 len  = 0;
	unsigned char*		 data = NULL;

	if(packet->header.length > 0) {
		z_stream      strm;
		int	      in    = packet->header.length;
		int	      seek  = 0;
		int	      flush = 0;
		unsigned char buf[CHUNK];

		strm.zalloc = Z_NULL;
		strm.zfree  = Z_NULL;
		strm.opaque = Z_NULL;
		if(deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 0;

		do {
			strm.avail_in = in <= CHUNK ? in : CHUNK;
			strm.next_in  = ((unsigned char*)packet->data) + seek;

			flush = in <= CHUNK ? Z_FINISH : Z_NO_FLUSH;

			do {
				int have;

				strm.avail_out = CHUNK;
				strm.next_out  = buf;

				deflate(&strm, flush);

				have = CHUNK - strm.avail_out;

				if(data == NULL) {
					data = malloc(have);
					memcpy(data, buf, have);
				} else {
					unsigned char* old = data;

					data = malloc(len + have);
					memcpy(data, old, len);
					memcpy(data + len, buf, have);

					free(data);
				}

				len += have;
			} while(strm.avail_out == 0);

			seek += CHUNK;
		} while(flush != Z_FINISH);

		deflateEnd(&strm);
	}

	memcpy(&h, &packet->header, sizeof(h));
	h.length = htons(len);

	r = gbnet_fd_send(fd, &h, sizeof(h));
	if(r != sizeof(h)) return 0;

	if(data != NULL) {
		r = gbnet_fd_send(fd, data, len);
		if(r != sizeof(h)) {
			free(data);
			return 0;
		}
		free(data);
	}

	return sizeof(h) + len;
}

int gbnet_packet_recv(int fd, gbnet_packet_t* packet) {
	gbnet_packet_header_t h;

	if(gbnet_fd_recv(fd, &h, sizeof(h), 500) != sizeof(h)) {
		packet->data = NULL;
		return 0;
	}

	memcpy(&packet->header, &h, sizeof(h));
	packet->header.length = ntohs(packet->header.length);
	packet->data	      = NULL;
	if(packet->header.length > 0) {
		unsigned char* data = malloc(packet->header.length);
		z_stream       strm;
		int	       in = packet->header.length;
		int	       ret;
		int	       seek = 0;
		int	       len  = 0;
		unsigned char  buf[CHUNK];
		if(gbnet_fd_recv(fd, data, packet->header.length, 50) != packet->header.length) {
			free(data);
			return 0;
		}

		strm.zalloc   = Z_NULL;
		strm.zfree    = Z_NULL;
		strm.opaque   = Z_NULL;
		strm.avail_in = 0;
		strm.next_in  = Z_NULL;

		if(inflateInit(&strm) != Z_OK) {
			free(data);
			return 0;
		}

		do {
			strm.avail_in = in <= CHUNK ? in : CHUNK;
			strm.next_in  = data + seek;

			do {
				int have;

				strm.avail_out = CHUNK;
				strm.next_out  = buf;

				ret = inflate(&strm, Z_NO_FLUSH);

				if(ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
					inflateEnd(&strm);
					if(packet->data != NULL) {
						free(packet->data);
						packet->data = NULL;
					}
					free(data);
					return 0;
				}

				have = CHUNK - strm.avail_out;

				if(packet->data == NULL) {
					packet->data = malloc(have);
					memcpy(packet->data, buf, have);
				} else {
					unsigned char* old = packet->data;

					packet->data = malloc(len + have);
					memcpy(packet->data, old, len);
					memcpy(packet->data + len, buf, have);

					free(packet->data);
				}

				len += have;
			} while(strm.avail_out == 0);
		} while(ret != Z_STREAM_END);

		inflateEnd(&strm);

		free(data);

		packet->header.length = len;
	}

	return sizeof(h) + packet->header.length;
}

int gbnet_fd_send(int fd, const void* buf, int length) {
	struct pollfd pfd;
	pfd.fd	   = fd;
	pfd.events = POLLOUT | POLLPRI;

	poll(&pfd, 1, 0);
	if(!(pfd.revents & POLLOUT)) return 0;

	return send(fd, buf, length, 0);
}

int gbnet_fd_recv(int fd, void* buf, int length, int to) {
	struct pollfd  pfd;
	int	       ret;
	unsigned char* b = buf;
	int	       i = 0;
	time_t	       t = time(NULL);

	pfd.fd	   = fd;
	pfd.events = POLLIN | POLLPRI;

	while(i < length) {
		/* i think 500 is reasonable timeout. no? */
		ret = poll(&pfd, 1, to);

		if((time(NULL) - t) >= 5) return i; /* we waited enough! kill the connection */
		if(ret <= 0) return i;

		ret = recv(fd, &b[i], 1, 0);
		if(ret < 1) return i;
		i++;
	}
	return i;
}
