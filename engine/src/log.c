#include <GearSrc/Log.h>

void GSLog(int level, const char* fmt, ...) {
	va_list va;
	FILE*	out = stdout;

#ifdef _WIN32
	fprintf(out, "[GearSrc|");

	if(level == GSLogInfo) {
		fprintf(out, "INFO ");
	} else if(level == GSLogWarn) {
		fprintf(out, "WARN ");
	} else if(level == GSLogError) {
		fprintf(out, "ERROR");
	}
#else
	fprintf(out, "[\x1b[mGearSrc\x1b[m|");

	if(level == GSLogInfo) {
		fprintf(out, "\x1b[1mINFO\x1b[m ");
	} else if(level == GSLogWarn) {
		fprintf(out, "\x1b[1m\x1b[33mWARN\x1b[m ");
	} else if(level == GSLogError) {
		fprintf(out, "\x1b[1m\x1b[31mERROR\x1b[m");
	}
#endif

	fprintf(out, "] ");

	va_start(va, fmt);
	vfprintf(out, fmt, va);
	va_end(va);

	fprintf(out, "\n");
}
