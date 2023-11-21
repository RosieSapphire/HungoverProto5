#include <math.h>

#include "engine/vector.h"

f32 vector_dot(const f32 *a, const f32 *b, const u8 comp)
{
	f32 total = 0.0f;

	for (int i = 0; i < comp; i++)
		total += a[i] * b[i];

	return (total);
}

f32 vector_magnitude_sqr(const f32 *x, const u8 comp)
{
	return (vector_dot(x, x, comp));
}

f32 vector_magnitude(const f32 *x, const u8 comp)
{
	return (sqrtf(vector_magnitude_sqr(x, comp)));
}

f32 vector_normalize(f32 *x, const u8 comp)
{
	f32 mag = vector_magnitude(x, comp);

	if (!mag)
		return (0.0f);

	for (int i = 0; i < comp; i++)
		x[i] /= mag;

	return (mag);
}

/**
 * vector3_cross - Vector 3 Cross Product
 * @a: Vector A
 * @b: Vector B
 * @o: Vector Output
 */
void vector3_cross(const f32 *a, const f32 *b, f32 *o)
{
	o[0] = a[1] * b[2] - a[2] * b[1];
	o[1] = a[2] * b[0] - a[0] * b[2];
	o[2] = a[0] * b[1] - a[1] * b[0];
}
