/**
 * @file src/engine/vector_math.c
 */

#include "engine/util.h"
#include "engine/vector.h"

/**
 * Adds two vectors
 * @param[in] a Vector A
 * @param[in] b Vector B
 * @param[out] c Vector Output
 * @param[in] comp Number of Components
 */
void vector_add(const f32 *a, const f32 *b, f32 *c, const u8 comp)
{
	for (int i = 0; i < comp; i++)
		c[i] = a[i] + b[i];
}

/**
 * Subtracts two vectors
 * @param[in] a Vector A
 * @param[in] b Vector B
 * @param[out] c Vector Output
 * @param[in] comp Number of Components
 */
void vector_sub(const f32 *a, const f32 *b, f32 *c, const u8 comp)
{
	for (int i = 0; i < comp; i++)
		c[i] = a[i] - b[i];
}

/**
 * Scales a vector by a scalar
 * @param[in] Input Vector
 * @param[in] s Scalar
 * @param[out] Output Vector
 * @param[in] comp Number of Components
 */
void vector_scale(const f32 *in, f32 s, f32 *out, const u8 comp)
{
	for (int i = 0; i < comp; i++)
		out[i] = in[i] * s;
}

/**
 * Lerps between two vectors
 * @param[in] a Vector A
 * @param[in] b Vector B
 * @param[in] t Interpolation Factor
 * @param[out] o Vector Output
 * @param[in] comp Number of Components
 */
void vector_lerp(const f32 *a, const f32 *b, f32 t, f32 *o, const u8 comp)
{
	for (int i = 0; i < comp; i++)
		o[i] = lerpf(a[i], b[i], t);
}

/**
 * Smoothly Interpolates between two vectors
 * @param[in] a Vector A
 * @param[in] b Vector B
 * @param[in] t Interpolation Factor
 * @param[out] o Vector Output
 * @param[in] comp Number of Components
 */
void vector_smooth(f32 *a, f32 *b, f32 t, f32 *o, const u8 comp)
{
	for (int i = 0; i < comp; i++)
		o[i] = smoothf(a[i], b[i], t);
}
