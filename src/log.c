#include <af_common.h>

static void af_common_log(const char* type, const char* fmt, va_list va) {
	printf("[%.4s] ", type);
	vprintf(fmt, va);
}

void af_common_log_info(const char* fmt, ...) {
	va_list va;

	va_start(va, fmt);
	af_common_log("INFO", fmt, va);
	va_end(va);
}
