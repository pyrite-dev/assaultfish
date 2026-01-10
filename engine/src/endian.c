#include <GearBox/Endian.h>

#ifdef GBEndianHostLittle
static void swap(void* from, void* to, int size) {
	unsigned char* f = from;
	unsigned char* t = to;

	int i;

	for(i = 0; i < size; i++) {
		t[size - 1 - i] = f[i];
	}
}

#define SWAP(in, out) swap(&in, &out, sizeof(in))

GBI16 GBEndianSwapI16BE(GBI16 in) {
	GBI16 out;

	SWAP(in, out);

	return out;
}

GBI32 GBEndianSwapI32BE(GBI32 in) {
	GBI32 out;

	SWAP(in, out);

	return out;
}

GBI64 GBEndianSwapI64BE(GBI64 in) {
	GBI64 out;

	SWAP(in, out);

	return out;
}

GBU16 GBEndianSwapU16BE(GBU16 in) {
	GBU16 out;

	SWAP(in, out);

	return out;
}

GBU32 GBEndianSwapU32BE(GBU32 in) {
	GBU32 out;

	SWAP(in, out);

	return out;
}

GBU64 GBEndianSwapU64BE(GBU64 in) {
	GBU64 out;

	SWAP(in, out);

	return out;
}
#endif
