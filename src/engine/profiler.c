#include <string.h>
#include <libdragon.h>

#include "engine/types.h"
#include "engine/profiler.h"

u32 profile_start, profile_end;
char profile_name[32];

/**
 * profiler_begin - Begins the Profiler on the code you want to profile
 * @name: Name of function you're profiling
 */
void profiler_begin(const char *name)
{
	profile_start = get_ticks();
	strncpy(profile_name, name, 31);
}

/**
 * profiler_end_print - Ends the Profiler and prints out the time it took
 */
void profiler_end_print(void)
{
	u32 profile_delta;

	profile_end = get_ticks();
	profile_delta = TICKS_DISTANCE(profile_start, profile_end);
	debugf("'%s' took %f seconds (%ld ticks)\n", profile_name,
		(f32)profile_delta / (f32)TICKS_PER_SECOND, profile_delta);
	memset(profile_name, 0, 32);
}
