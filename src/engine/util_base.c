/**
 * @file src/engine/util_base.c
 */

#include <stdlib.h>
#include <GL/gl.h>

#include "engine/config.h"
#include "engine/vector.h"
#include "engine/util.h"

/**
 * Clamps a float between two values
 * @param[in] x Float value
 * @param[in] min Minimum
 * @param[in] max Maximum
 * @return Clamped Float
 */
f32 clampf(f32 x, f32 min, f32 max)
{
	if (x > max)
		return (max);

	if (x < min)
		return (min);

	return (x);
}

/**
 * Clamps an integer between two values
 * @param[in] x integer value
 * @param[in] min Minimum
 * @param[in] max Maximum
 * @return Clamped integer
 */
int clampi(int x, int min, int max)
{
	if (x > max)
		return (max);

	if (x < min)
		return (min);

	return (x);
}

/**
 * Lerps between two values
 * @param[in] a Value A
 * @param[in] b Value B
 * @param[in] t Interpolation
 * @return Lerped Value
 */
f32 lerpf(f32 a, f32 b, f32 t)
{
	return (a + (b - a) * clampf(t, 0, 1));
}

/**
 * Smoothly interpolates between two values
 * @param[in] a Value A
 * @param[in] b Value B
 * @param[in] t Interpolation
 * @return Smooth-interped Value
 */
f32 smoothf(f32 a, f32 b, f32 t)
{
	return (lerpf(a, b, t * t * (3 - 2 * t)));
}

/**
 * Effectively Modulo for Floats
 * @param[in] x Value
 * @param[in] max Maximum
 * @return: Wrapped Value
 */
f32 wrapf(f32 x, f32 max)
{
	while (x > max)
		x -= max;

	return (x);
}

f32 randf01(void)
{
	const u16 intermediate = rand() & 0xFFFF;

	return ((f32)intermediate / (f32)0xFFFF);
}
