#include <math.h>

#include "engine/vector.h"

/**
 * Vector get Magnitude Squared
 * @param[in] a Vector
 * @param[in] comp Number of Components
 * @return Vectors magnitude (squared)
 */
f32 vector_dot(const f32 *a, const f32 *b, const u8 comp)
{
	f32 total = 0.0f;

	for (int i = 0; i < comp; i++)
		total += a[i] * b[i];

	return (total);
}

/**
 * Vector get Magnitude Squared
 * @param[in] a Vector
 * @param[in] comp Number of Components
 * @return Vectors magnitude (squared)
 */
f32 vector_magnitude_sqr(const f32 *x, const u8 comp)
{
	return (vector_dot(x, x, comp));
}

/**
 * Vector get Magnitude
 * @param[in] a Vector
 * @param[in] comp Number of Components
 * @return Vectors magnitude
 */
f32 vector_magnitude(const f32 *x, const u8 comp)
{
	return (sqrtf(vector_magnitude_sqr(x, comp)));
}

/**
 * Vector Normalize
 * @param[in,out] a Vector
 * @param[in] comp Number of Components
 */
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
 * Vector 3 Cross Product
 * @param[in] a Vector A
 * @param[in] b Vector B
 * @param[out] o Vector Output
 */
void vector3_cross(const f32 *a, const f32 *b, f32 *o)
{
	o[0] = a[1] * b[2] - a[2] * b[1];
	o[1] = a[2] * b[0] - a[0] * b[2];
	o[2] = a[0] * b[1] - a[1] * b[0];
}
