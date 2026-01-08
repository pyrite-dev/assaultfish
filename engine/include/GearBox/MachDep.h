#ifndef __GEARBOX_MACHDEP_H__
#define __GEARBOX_MACHDEP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#ifdef _GEARBOX
#include <gbnet.h>
#endif

#if defined(_GEARBOX) && defined(_WIN32)
#define GBDECL extern __declspec(dllexport)
#elif defined(_WIN32)
#define GBDECL extern __declspec(dllimport)
#else
#define GBDECL extern
#endif

#endif
