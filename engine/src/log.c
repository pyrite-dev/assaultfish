#include <GearSrc/Log.h>

#include <stb_ds.h>

void GSLog(GSEngine engine, int level, const char* fmt, ...) {
	va_list va;
	FILE*	out = stdout;

#ifndef DEBUG
	if(level == GSLogDebug) return;
#endif

#ifdef _WIN32
	fprintf(out, "[GearSrc|");

	if(level == GSLogInfo) {
		fprintf(out, "INFO ");
	} else if(level == GSLogWarn) {
		fprintf(out, "WARN ");
	} else if(level == GSLogError) {
		fprintf(out, "ERROR");
	} else if(level == GSLogDebug) {
		fprintf(out, "DEBUG");
	}
#else
	fprintf(out, "[\x1b[mGearSrc\x1b[m|");

	if(level == GSLogInfo) {
		fprintf(out, "\x1b[1mINFO\x1b[m ");
	} else if(level == GSLogWarn) {
		fprintf(out, "\x1b[1m\x1b[33mWARN\x1b[m ");
	} else if(level == GSLogError) {
		fprintf(out, "\x1b[1m\x1b[31mERROR\x1b[m");
	} else if(level == GSLogDebug) {
		fprintf(out, "\x1b[1m\x1b[34mDEBUG\x1b[m");
	}
#endif

	fprintf(out, "] ");

	va_start(va, fmt);
	vfprintf(out, fmt, va);
	va_end(va);

	if(engine != NULL) {
		GSEngineLog log;
		char	    str[6];

		log.level = level;

		va_start(va, fmt);
		vsprintf(log.log, fmt, va);
		va_end(va);

		if(engine->param->get_tick == NULL) {
			log.tick = 0;
		} else {
			log.tick = engine->param->get_tick();
		}

		arrput(engine->log, log);
	}

	fprintf(out, "\n");
}
