#include <GearSrc/Endian.h>

#ifdef GSEndianHostLittle
static void swap(void* from, void* to, int size) {
	unsigned char* f = from;
	unsigned char* t = to;

	int i;

	for(i = 0; i < size; i++) {
		t[size - 1 - i] = f[i];
	}
}

#define SWAP(in, out) swap(&in, &out, sizeof(in))

GSI16 GSEndianSwapI16BE(GSI16 in) {
	GSI16 out;

	SWAP(in, out);

	return out;
}

GSI32 GSEndianSwapI32BE(GSI32 in) {
	GSI32 out;

	SWAP(in, out);

	return out;
}

GSI64 GSEndianSwapI64BE(GSI64 in) {
	GSI64 out;

	SWAP(in, out);

	return out;
}

GSU16 GSEndianSwapU16BE(GSU16 in) {
	GSU16 out;

	SWAP(in, out);

	return out;
}

GSU32 GSEndianSwapU32BE(GSU32 in) {
	GSU32 out;

	SWAP(in, out);

	return out;
}

GSU64 GSEndianSwapU64BE(GSU64 in) {
	GSU64 out;

	SWAP(in, out);

	return out;
}
#endif
