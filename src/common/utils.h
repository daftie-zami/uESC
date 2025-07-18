#pragma once

#include <stdio.h>

#define __DEBUG__

#ifdef __DEBUG__
#define _DPRINTF(fmt, ...) \
	do { printf(fmt, ## __VA_ARGS__); } while (0)
#else
#define _DPRINTF(fmt, ...) \
	do { } while (0)
#endif