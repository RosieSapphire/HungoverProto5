/**
 * @file src/engine/camera.c
 */

#include <math.h>
#include <GL/glu.h>

#include "engine/util.h"
#include "engine/vector.h"
#include "engine/camera.h"

/**
 * Initializes Camera
 * @param[out] c Camera to Initialize
 */
void camera_init(struct camera *c)
{
	vector_zero(c->eye_last, 3);
	vector_zero(c->eye, 3);
	vector_zero(c->angles_last, 2);
	vector_zero(c->angles, 2);
}

/**
 * Updates Camera Eye and Angles
 * @param[in,out] c Camera to Update
 * @param[in] iparms Input Parameters
 */
void camera_update(struct camera *c, const struct input_parms iparms)
{
	/*
	 * angle changing
	 */
	vector_copy(c->angles_last, c->angles, 2);
	c->angles[0] += (f32)iparms.stick.stick_x * 0.001f;
	c->angles[1] -= (f32)iparms.stick.stick_y * 0.001f;

	/*
	 * position changing
	 */
	f32 focus[3], forw[3] = {0}, side[3] = {0}, move[3] = {0};
	const f32 move_forw =
		(f32)(iparms.held.c_up - iparms.held.c_down) * 0.1f;
	const f32 move_side =
		(f32)(iparms.held.c_right - iparms.held.c_left) * 0.1f;
	const u8 go_fast = (iparms.held.z > 0) + 1;

	camera_get_focus_now(c, focus);
	vector_sub(focus, c->eye, forw, 3);
	vector_normalize(forw, 3);
	vector3_cross(forw, (f32[3]) {0, 1, 0}, side);
	vector_scale(forw, move_forw * go_fast, forw, 3);
	vector_scale(side, move_side * go_fast, side, 3);

	vector_copy(c->eye_last, c->eye, 3);
	vector_add(move, forw, move, 3);
	vector_add(move, side, move, 3);
	vector_add(c->eye, move, c->eye, 3);
}

/**
 * Gets Camera's Focus on Current Frame
 * @param[in] c Camera
 * @param[out] out Focus Now Output
 */
void camera_get_focus_now(const struct camera *c, f32 *out)
{
	const f32 cospl = cosf(c->angles[1]);

	vector_copy(out, c->eye, 3);
	out[0] += cosf(c->angles[0]) * cospl;
	out[1] += sinf(c->angles[1]);
	out[2] += sinf(c->angles[0]) * cospl;
}

void camera_get_focus_now_offset(const struct camera *c,
				 f32 *out, const f32 *angle_offset)
{
	const f32 cospl = cosf(c->angles[1] + angle_offset[1]);

	vector_copy(out, c->eye, 3);
	out[0] += cosf(c->angles[0] + angle_offset[0]) * cospl;
	out[1] += sinf(c->angles[1] + angle_offset[1]);
	out[2] += sinf(c->angles[0] + angle_offset[0]) * cospl;
}

/**
 * Gets Camera Value's Focus between Two Frames
 * @param[in] eye_lerp Lerped Eye Position
 * @param[in] angles_lerp Lerped Angles
 * @param[out] out Focus Lerp Output
 */
void camera_get_focus_lerp(f32 *eye_lerp, f32 *angles_lerp, f32 *out)
{
	const f32 cospl = cosf(angles_lerp[1]);

	vector_copy(out, eye_lerp, 3);
	out[0] += cosf(angles_lerp[0]) * cospl;
	out[1] += sinf(angles_lerp[1]);
	out[2] += sinf(angles_lerp[0]) * cospl;
}

/**
 * Sets up OpenGL's View Matrix with Camera
 * @param[in] c Camera
 * @param[in] subtick Delta Between Frames
 */
void camera_view_matrix_setup(const struct camera *c, f32 subtick)
{
	f32 eye_lerp[3];
	f32 angles_lerp[2];
	f32 focus_lerp[3];

	vector_lerp(c->eye_last, c->eye, subtick, eye_lerp, 3);
	vector_lerp(c->angles_last, c->angles, subtick, angles_lerp, 2);
	camera_get_focus_lerp(eye_lerp, angles_lerp, focus_lerp);

	gluLookAt(eye_lerp[0], eye_lerp[1], eye_lerp[2],
		  focus_lerp[0], focus_lerp[1], focus_lerp[2],
		0, 1, 0);
}
