#include <GearBox/Log.h>

void GBLog(int level, const char* fmt, ...) {
	va_list va;
	FILE*	out = stdout;

#ifdef _WIN32
#else
	fprintf(out, "[\x1b[mGearBox\x1b[m|");

	if(level == GBLogInfo) {
		fprintf(out, "\x1b[1mINFO\x1b[m ");
	} else if(level == GBLogWarn) {
		fprintf(out, "\x1b[1m\x1b[33mWARN\x1b[m ");
	} else if(level == GBLogError) {
		fprintf(out, "\x1b[1m\x1b[31mERROR\x1b[m");
	}
#endif

	fprintf(out, "] ");

	va_start(va, fmt);
	vfprintf(out, fmt, va);
	va_end(va);

	fprintf(out, "\n");
}
