#include <GL/glu.h>

#include "engine/util.h"
#include "engine/vector.h"
#include "engine/player.h"

/**
 * player_camera_view_matrix_setup - Sets up View Matrix for OpenGL with Cam
 * @p: Player whose camera to reference
 * @subtick: Subtick between Frames
 */
void player_camera_view_matrix_setup(const struct player *p,
				     const f32 subtick)
{
	f32 eye_lerp[3];
	f32 angles_lerp[2];
	f32 focus_lerp[3];

	f32 recoil_vec[2];
	f32 recoil_amnt_lerp =
		lerpf(p->recoil_amnt_last, p->recoil_amnt, subtick);

	vector_copy(recoil_vec, p->recoil_dir, 2);
	vector_scale(recoil_vec, recoil_amnt_lerp, 2);

	vector_lerp(p->view.eye_last, p->view.eye,
	     subtick, eye_lerp, 3);
	vector_lerp(p->view.angles_last, p->view.angles,
	     subtick, angles_lerp, 2);
	vector_add(angles_lerp, recoil_vec, angles_lerp, 2);
	camera_get_focus_lerp(eye_lerp, angles_lerp, focus_lerp);

	gluLookAt(eye_lerp[0], eye_lerp[1], eye_lerp[2],
		focus_lerp[0], focus_lerp[1], focus_lerp[2],
		0, 1, 0);
}

/**
 * _player_camera_turn_offset_update - Updates turn offset for item in hand
 * @p: Player to Update angle for
 */
static void _player_camera_turn_offset_update(struct player *p)
{

	const f32 turn_vec[2] = {
		p->view.angles_last[0] - p->view.angles[0],
		p->view.angles_last[1] - p->view.angles[1],
	};
	const f32 turn_off_mag = vector_magnitude(p->turn_offset, 2);

	if (turn_off_mag > 0.0f)
	{
		f32 newturn = turn_off_mag - (turn_off_mag * 0.2f);

		if (newturn < 0.001f)
			newturn = 0;
		newturn /= turn_off_mag;
		vector_scale(p->turn_offset, newturn, 2);
	}
	else
	{
		vector_zero(p->turn_offset, 3);
	}

	vector_add(p->turn_offset, turn_vec, p->turn_offset, 2);
	const f32 turn_off_max = 1.0f;

	if (turn_off_mag > turn_off_max)
		vector_scale(p->turn_offset, turn_off_max / turn_off_mag, 2);
}

/**
 * player_camera_look_update - Updates look angles for Player
 * @p: Player to Update angles for
 * @iparms: Input Parameters
 */
void player_camera_look_update(struct player *p,
			       const struct input_parms iparms)
{
	f32 stick[2] = {
		(f32)iparms.stick.stick_x * 0.0035f,
		(f32)iparms.stick.stick_y * 0.0035f,
	};

	if (fabsf(stick[0]) < 0.01f)
		stick[0] = 0.0f;

	if (fabsf(stick[1]) < 0.01f)
		stick[1] = 0.0f;

	p->view.angles[0] += stick[0];
	p->view.angles[1] -= stick[1];
	p->view.angles[1] = clampf(p->view.angles[1], -1.57f, 1.57f);

	_player_camera_turn_offset_update(p);
}
