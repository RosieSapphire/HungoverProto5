/**
 * @file include/engine/profiler.h
 */

#ifndef _ENGINE_PROFILER_H_
#define _ENGINE_PROFILER_H_

void profiler_begin(const char *fmt, ...);
void profiler_end_print(void);

#endif /* _ENGINE_PROFILER_H_ */
