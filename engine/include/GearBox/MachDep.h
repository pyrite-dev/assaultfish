#ifndef __GEARBOX_MACHDEP_H__
#define __GEARBOX_MACHDEP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#if defined(_MILSKO) && defined(_WIN32)
#define GBDECL extern __declspec(dllexport)
#elif defined(_WIN32)
#define GBDECL extern __declspec(dllimport)
#else
#define GBDECL extern
#endif

#endif
