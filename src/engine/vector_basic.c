#include <string.h>
#include <libdragon.h>

#include "engine/vector.h"

/**
 * vector_copy - Copies a Vector
 * @dst: Vector Destination
 * @src: Vector Source
 * @comp: Number of Components
 *
 * Description: Copies a Vector from SRC to DST
 */
void vector_copy(f32 *dst, const f32 *src, const u8 comp)
{
	memcpy(dst, src, sizeof(f32) * comp);
}

/**
 * vector_zero - Zeroes out a Vector
 * @vec: Vector (In/Out)
 * @comp: Number of Components
 *
 * Description: Zeroes out a Vector
 */
void vector_zero(f32 *vec, const u8 comp)
{
	memset(vec, 0, comp);
}

/**
 * vector_print - Prints a Vector
 * @x: Vector (In)
 * @comp: Number of Components
 *
 * Description: Prints a Vector using debugf
 */
void vector_print(f32 *x, const u8 comp)
{
	for (int i = 0; i < comp; i++)
		debugf("%f ", x[i]);
	debugf("\n");
}
