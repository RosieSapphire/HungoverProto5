/**
 * @file src/engine/vector_basic.c
 */

#include <string.h>
#include <libdragon.h>

#include "engine/util.h"
#include "engine/vector.h"

/**
 * Copies a Vector
 * @param[out] dst Vector Destination
 * @param[in] src Vector Source
 * @param[in] comp Number of Components
 */
void vector_copy(f32 *dst, const f32 *src, const u8 comp)
{
	for (u8 i = 0; i < comp; i++)
		dst[i] = src[i];
}

/**
 * Zeroes out a Vector
 * @param[in,out] vec Vector
 * @param[in] comp Number of Components
 */
void vector_zero(f32 *vec, const u8 comp)
{
	memset(vec, 0, comp);
}

/**
 * Prints a Vector
 * @param[in] x Vector
 * @param[in] comp Number of Components
 */
void vector_print(f32 *x, const u8 comp)
{
	for (int i = 0; i < comp; i++)
		debugf("%f ", x[i]);
	debugf("\n");
}

void vector3_rotate_z(f32 *in, float angle, f32 *out)
{
	const f32 costheta = cos(angle * TO_RADIANS);
	const f32 sintheta = sin(angle * TO_RADIANS);

	out[0] = in[0] * costheta - in[1] * sintheta;
	out[1] = in[0] * sintheta + in[1] * costheta;
	out[2] = in[2];
}
