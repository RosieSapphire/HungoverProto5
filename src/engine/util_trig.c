/**
 * @file src/engine/util_trig.c
 */

#include <GL/gl.h>

#include "engine/config.h"
#include "engine/vector.h"
#include "engine/util.h"

/**
 * Sets up the Projection Matrix for OpenGL
 */
void projection_setup(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-CONF_NEAR * CONF_ASPECT, CONF_NEAR * CONF_ASPECT,
		  -CONF_NEAR, CONF_NEAR, CONF_NEAR, CONF_FAR);
}

/**
 * Lerps between quaternions
 * @param[in] a Quaternion A
 * @param[in] b Quaternion B
 * @param[in] c Quaternion Out
 * @param[in] t Interpolation
 */
void quat_lerp(const f32 *a, const f32 *b, f32 *c, const f32 t)
{
	f32 out_scale = (vector_dot(a, b, 4) >= 0) ? 1.0f : -1.0f;

	for (int i = 0; i < 4; i++)
		c[i] = ((1 - t) * a[i]) + (out_scale * t * b[i]);

	vector_normalize(c, 4);
}

/**
 * Matrix Multiply
 * @param[in] a Matrix A
 * @param[in] b Matrix B
 * @param[out] o Matrix Out
 */
void matrix_mult(const f32 a[4][4], const f32 b[4][4], f32 o[4][4])
{
	f32 a_tmp[4][4];

	memcpy(a_tmp, a, sizeof(f32) * 16);
	memset(o, 0, sizeof(f32) * 16);
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			for (int ind = 0; ind < 4; ind++)
				o[col][row] += a_tmp[ind][row] * b[col][ind];
}

/**
 * Matrix Debugging
 * @param[in] m Matrix to Debug
 */
void matrix_debug(const f32 m[4][4])
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			debugf("%f ", m[row][col]);
		}
		debugf("\n");
	}
	debugf("\n");
}

/**
 * Position from Matrix
 * @param[in] mat The Matrix (Starring Keanu Reeves)
 * @param[out] pos Position Vector Output
 */
void pos_from_mat(const f32 *mat, f32 *pos)
{
	pos[0] = mat[12];
	pos[1] = mat[13];
	pos[2] = mat[14];
}
