#include "engine/sfx.h"
#include "engine/vector.h"
#include "engine/util.h"
#include "engine/raycast.h"
#include "engine/player.h"

#define ACCEL_RATE 0.1f
#define MAX_VEL    0.8f
#define FRICTION   4.0f

void player_friction(struct player *p)
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

void player_accelerate(struct player *p, const struct input_parms iparms)
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

	p->headbob_timer_last = p->headbob_timer;
	p->headbob_timer += vector_magnitude(p->vel, 3);

	if (signbit(cosf(p->headbob_timer)) !=
		signbit(cosf(p->headbob_timer_last)))
	{
		const f32 vol = sqrtf(velmagsqr) * 2;
		const u16 freq = CONF_AUDIO_FREQ + (rand() & 0xFFF);

		mixer_ch_set_freq(SFXC_FOOTSTEPS, freq);
		mixer_ch_set_vol(SFXC_FOOTSTEPS, vol, vol);
		wav64_play(&footstep2_sfx, SFXC_FOOTSTEPS);
	}
}
