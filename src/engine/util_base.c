#include <GL/gl.h>

#include "engine/config.h"
#include "engine/vector.h"
#include "engine/util.h"

/**
 * clampf - Clamps a float between two values
 * @x: Float value
 * @min: Minimum
 * @max: Maximum
 *
 * Description: Clamps a float between a minimum and a maximum
 * Return: Clamped Float
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
 * clampi - Clamps an integer between two values
 * @x: Integer value
 * @min: Minimum
 * @max: Maximum
 *
 * Description: Clamps an integer between a minimum and a maximum
 * Return: Clamped Integer
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
 * lerpf - Lerps between two values
 * @a: Value A
 * @b: Value B
 * @t: Interpolation
 *
 * Description: Linearly Interpolates between two values
 * Return: Lerped Value
 */
f32 lerpf(f32 a, f32 b, f32 t)
{
	return (a + (b - a) * clampf(t, 0, 1));
}

/**
 * smoothf - Smooth Interps between two values
 * @a: Value A
 * @b: Value B
 * @t: Interpolation
 *
 * Description: Smoothly Interpolates between two values
 * Return: Smoothly Interpolated Value
 */
f32 smoothf(f32 a, f32 b, f32 t)
{
	return (lerpf(a, b, t * t * (3 - 2 * t)));
}

/**
 * wrapf - Effectively Modulo for Floats
 * @x: Value
 * @max: Maximum
 *
 * Description: Effectively Modulo for Floats, but not expensive
 * Return: Wrapped Value
 */
f32 wrapf(f32 x, f32 max)
{
	while (x > max)
		x -= max;

	return (x);
}
