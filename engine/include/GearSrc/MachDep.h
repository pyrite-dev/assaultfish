#ifndef __GEARSRC_MACHDEP_H__
#define __GEARSRC_MACHDEP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#ifndef _WIN32
#include <unistd.h>
#endif

#if defined(_GEARSRC) && defined(_WIN32)
#define GSDECL extern __declspec(dllexport)
#elif defined(_WIN32)
#define GSDECL extern __declspec(dllimport)
#else
#define GSDECL extern
#endif

#endif
