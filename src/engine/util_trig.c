#include <GL/gl.h>

#include "engine/config.h"
#include "engine/vector.h"
#include "engine/util.h"

/**
 * projection_setup - Sets up the Projection Matrix
 *
 * Description: Sets up the Projection Matrix using Libdragon's OpenGL SDK
 */
void projection_setup(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-CONF_NEAR * CONF_ASPECT, CONF_NEAR * CONF_ASPECT,
	   -CONF_NEAR, CONF_NEAR, CONF_NEAR, CONF_FAR);
}

/**
 * quat_lerp - Lerps between quaternions
 * @a: Quaternion A
 * @b: Quaternion B
 * @c: Quaternion Out
 * @t: Interpolation
 *
 * Description: Linearly Interpolates between two quaternions
 */
void quat_lerp(const f32 *a, const f32 *b, f32 *c, const f32 t)
{
	f32 out_scale = (vector_dot(a, b, 4) >= 0) ? 1.0f : -1.0f;

	for (int i = 0; i < 4; i++)
		c[i] = ((1 - t) * a[i]) + (out_scale * t * b[i]);

	vector_normalize(c, 4);
}

/**
 * pos_from_mat - Position from Matrix
 * @mat: The Matrix (Starring Keanu Reeves)
 * @pos: Position Vector Output
 *
 * Description: Gets position from a 4x4 matrix
 */
void pos_from_mat(const f32 *mat, f32 *pos)
{
	pos[0] = mat[12];
	pos[1] = mat[13];
	pos[2] = mat[14];
}
