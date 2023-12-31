#ifndef _ENGINE_UTIL_H_
#define _ENGINE_UTIL_H_

/**
 * @file include/engine/util.h
 */

#include "engine/types.h"

#define PI 3.14159265358979323846f ///< Precomputed pi value
#define PI_HALF 1.57079632679489661923f ///< Precomputed half-pi value

#define TO_RADIANS (PI / 180.0f) ///< Multiply to convert to radians
#define TO_DEGREES (180.0f / PI) ///< Multiply to convert to degrees
#define SQRT_3 (1.73205080756887729353f) ///< Square Root of 3

/*
 * Base
 */
f32 clampf(f32 x, f32 min, f32 max);
int clampi(int x, int min, int max);
f32 lerpf(f32 a, f32 b, f32 t);
f32 smoothf(f32 a, f32 b, f32 t);
f32 wrapf(f32 x, f32 max);
f32 randf01(void);

/*
 * Trig
 */
void projection_setup(void);
void quat_lerp(const f32 *a, const f32 *b, f32 *c, const f32 t);
void matrix_mult(const f32 a[4][4], const f32 b[4][4], f32 o[4][4]);
void matrix_debug(const f32 m[4][4]);
void pos_from_mat(const f32 *mat, f32 *pos);

#endif /* _ENGINE_UTIL_H_ */
