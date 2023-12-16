/**
 * @file src/engine/profiler.c
 */

#include <stdarg.h>
#include <string.h>
#include <libdragon.h>

#include "engine/types.h"
#include "engine/profiler.h"
#include "engine/config.h"

static u32 profile_start, profile_end;
static char profile_name[32];

#ifdef CONF_DEBUG
void profiler_begin(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vsnprintf(profile_name, 32, fmt, args);
	va_end(args);

	profile_start = get_ticks();
}

void profiler_end_print(void)
{
	u32 profile_delta;

	profile_end = get_ticks();
	profile_delta = TICKS_DISTANCE(profile_start, profile_end);
	debugf("'%s' took %f seconds (%ld ticks)\n", profile_name,
	       (f32)profile_delta / (f32)TICKS_PER_SECOND, profile_delta);
	memset(profile_name, 0, 32);
}
#else
void profiler_begin(const char *fmt, ...)
{}

void profiler_end_print(void)
{}
#endif /* CONF_DEBUG */
