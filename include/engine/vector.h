#ifndef ENGINE_VECTOR_H_
#define ENGINE_VECTOR_H_

#include "engine/types.h"

/*
 * Basic
 */
void vector_copy(f32 *dst, const f32 *src, const u8 comp);
void vector_zero(f32 *vec, const u8 comp);
void vector_print(f32 *x, const u8 comp);

/*
 * Math
 */
void vector_add(const f32 *a, const f32 *b, f32 *c, const u8 comp);
void vector_sub(const f32 *a, const f32 *b, f32 *c, const u8 comp);
void vector_scale(f32 *x, f32 s, const u8 comp);
void vector_lerp(const f32 *a, const f32 *b, f32 t, f32 *o, const u8 comp);
void vector_smooth(f32 *a, f32 *b, f32 t, f32 *o, const u8 comp);

/*
 * Trig
 */
f32 vector_dot(const f32 *a, const f32 *b, const u8 comp);
f32 vector_magnitude_sqr(f32 *x, const u8 comp);
f32 vector_magnitude(f32 *x, const u8 comp);
f32 vector_normalize(f32 *x, const u8 comp);
void vector3_cross(const f32 *a, const f32 *b, f32 *o);

#endif /* ENGINE_VECTOR_H_ */
