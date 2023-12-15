/**
 * @file src/engine/vector_basic.c
 */

#include <string.h>
#include <libdragon.h>

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
