#include "engine/util.h"
#include "engine/vector.h"

/**
* vector_add - Adds two vectors
* @a: Vector A
* @b: Vector B
* @c: Vector Output
* @comp: Number of Components
*
* Description: Adds two vectors
*/
void vector_add(const f32 *a, const f32 *b, f32 *c, const u8 comp)
{
	for (int i = 0; i < comp; i++)
		c[i] = a[i] + b[i];
}

/**
* vector_sub - Subtracts two vectors
* @a: Vector A
* @b: Vector B
* @c: Vector Output
* @comp: Number of Components
*
* Description: Subtracts two vectors
*/
void vector_sub(const f32 *a, const f32 *b, f32 *c, const u8 comp)
{
	for (int i = 0; i < comp; i++)
		c[i] = a[i] - b[i];
}

/**
* vector_scale - Scales a Vector
* @x: Vector (In/Out)
* @s: Scalar
* @comp: Number of Components
*
* Description: Scales a Vector by an amount
*/
void vector_scale(f32 *x, f32 s, const u8 comp)
{
	for (int i = 0; i < comp; i++)
		x[i] = x[i] * s;
}

/**
* vector_lerp - Lerps between two Vectors
* @a: Vector A
* @b: Vector B
* @t: Interpolation
* @o: Vector Output
* @comp: Number of Components
*
* Description: Lerps between two Vectors
*/
void vector_lerp(const f32 *a, const f32 *b, f32 t, f32 *o, const u8 comp)
{
	for (int i = 0; i < comp; i++)
		o[i] = lerpf(a[i], b[i], t);
}

/**
* vector_smooth - Smoothly Interpolates between two Vectors
* @a: Vector A
* @b: Vector B
* @t: Interpolation
* @o: Vector Output
* @comp: Smoothly Interpolates between two Vectors
*
* Description: Lerps between two Vectors
*/
void vector_smooth(f32 *a, f32 *b, f32 t, f32 *o, const u8 comp)
{
	for (int i = 0; i < comp; i++)
		o[i] = smoothf(a[i], b[i], t);
}
