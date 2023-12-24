#ifndef _ENGINE_PROFILER_H_
#define _ENGINE_PROFILER_H_

/**
 * @file include/engine/profiler.h
 */

/**
 * Begins the Profiler on the code you want to profile
 * @param[in] fmt String Format
 * @param[in] ... Va args
 */
void profiler_begin(const char *fmt, ...);

/**
 * Ends the Profiler and prints out the time it took
 */
void profiler_end_print(void);

#endif /* _ENGINE_PROFILER_H_ */
