#include <GL/glu.h>

#include "engine/util.h"
#include "engine/vector.h"
#include "engine/player.h"

#define ACCEL_RATE 0.1f
#define MAX_VEL    0.8f
#define FRICTION   4.0f

void player_init(struct player *p, u8 items_equipped_flags)
{
	camera_init(&p->view);
	vector_copy(p->pos, p->view.eye, 3);
	vector_zero(p->vel, 3);
	p->item_flags = items_equipped_flags;
}

static void _player_camera_look_update(struct camera *c,
				  const struct input_parms iparms)
{
	vector_copy(c->angles_last, c->angles, 2);
	c->angles[0] += (f32)iparms.stick.stick_x * 0.005f;
	c->angles[1] -= (f32)iparms.stick.stick_y * 0.005f;
	c->angles[1] = clampf(c->angles[1], -1.57f, 1.57f);

}

static void _player_accelerate(struct player *p,
			       const struct input_parms iparms)
{
	f32 focus[3], forw[3] = {0}, side[3] = {0}, accel[3] = {0};
	const u8 go_fast = (iparms.held.r > 0) + 1;
	const f32 move_forw = (f32)(iparms.held.c_up - iparms.held.c_down);
	const f32 move_side = (f32)(iparms.held.c_right - iparms.held.c_left);

	camera_get_focus_now(&p->view, focus);
	vector_sub(focus, p->view.eye, forw, 3);
	forw[1] = 0.0f;
	vector_normalize(forw, 3);
	vector3_cross(forw, (f32[3]) {0, 1, 0}, side);
	vector_scale(forw, move_forw, 3);
	vector_scale(side, move_side, 3);
	vector_add(accel, forw, accel, 3);
	vector_add(accel, side, accel, 3);
	vector_normalize(accel, 3);
	vector_scale(accel, ACCEL_RATE * go_fast, 3);
	f32 velmagsqr = vector_magnitude_sqr(p->vel, 3);
	const f32 maxvelsqr = MAX_VEL * MAX_VEL * go_fast;

	if (velmagsqr > maxvelsqr)
		vector_scale(p->vel, maxvelsqr / velmagsqr, 3);

	vector_add(p->vel, accel, p->vel, 3);
}

static void _player_friction(struct player *p)
{
	const f32 speed = vector_magnitude(p->vel, 3);

	if (speed <= 0.0f)
	{
		vector_zero(p->vel, 3);
		return;
	}

	f32 newspeed = speed - (speed * ACCEL_RATE * FRICTION);

	if (newspeed < 0.001f)
		newspeed = 0;
	newspeed /= speed;
	vector_scale(p->vel, newspeed, 3);
}

void player_update(struct player *p, const struct input_parms iparms)
{

	_player_camera_look_update(&p->view, iparms);
	vector_copy(p->view.eye_last, p->pos, 3);
	_player_friction(p);
	_player_accelerate(p, iparms);
	vector_add(p->pos, p->vel, p->pos, 3);
	vector_copy(p->view.eye, p->pos, 3);
}

void player_camera_view_matrix_setup(const struct player *p, const f32 subtick)
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
