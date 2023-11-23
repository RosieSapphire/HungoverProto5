#include <GL/gl.h>
#include <GL/glu.h>

#include "engine/sfx.h"
#include "engine/util.h"
#include "engine/vector.h"
#include "engine/player.h"
#include "engine/raycast.h"
#include "engine/collision_mesh.h"

#define ACCEL_RATE 0.1f
#define MAX_VEL    0.8f
#define FRICTION   4.0f
#define HEIGHT     2.48f

void player_init(struct player *p, u8 items_equipped_flags)
{
	camera_init(&p->view);
	// vector_copy(p->view.eye, (f32[3]) {0, 0, 0}, 3);
	vector_copy(p->pos, p->view.eye, 3);
	vector_zero(p->vel, 3);
	vector_zero(p->vel_last, 3);
	vector_zero(p->turn_offset, 2);
	vector_zero(p->turn_offset_last, 2);
	p->item_flags = items_equipped_flags;
	p->headbob_timer = p->headbob_timer_last = 0;

	if (p->item_flags & ITEM_HAS_PISTOL)
	{
		scene_read_file(&p->pistol.s, "rom:/Pistol.scn");
		scene_anims_set_flags(&p->pistol.s, ANIM_LOOPS);
		scene_anims_set_frame(&p->pistol.s, 0);
		p->item_selected = ITEM_SELECT_PISTOL;
	}
}

void player_terminate(struct player *p)
{
	p->item_flags = ITEM_HAS_NONE;
	scene_destroy(&p->pistol.s);
}

static void _player_camera_look_update(struct player *p,
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

	/*
	 * Turning Camera
	 */
	p->view.angles[0] += stick[0];
	p->view.angles[1] -= stick[1];
	p->view.angles[1] = clampf(p->view.angles[1], -1.57f, 1.57f);

	const f32 turn_vec[2] = {
		p->view.angles_last[0] - p->view.angles[0],
		p->view.angles_last[1] - p->view.angles[1],
	};

	/*
	 * Item moving in hand from camera turn
	 */
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

static void _player_floor_collision(const struct collision_mesh *m,
				    struct player *p)
{
	f32 dir[3], eye[3], dist;

	for(u16 i = 0; i < m->num_verts / 3; i++) {
		vec3 v[3], a, b, n;

		vector_copy(v[0], m->verts[i * 3 + 0], 3);
		vector_copy(v[1], m->verts[i * 3 + 1], 3);
		vector_copy(v[2], m->verts[i * 3 + 2], 3);

		vector_sub(v[1], v[0], a, 3);
		vector_sub(v[2], v[0], b, 3);
		vector3_cross(a, b, n);
		vector_normalize(n, 3);

		vector_copy(eye, p->pos, 3);
		vector_add(eye, (f32[3]) {0, 0.5f, 0}, eye, 3);

		vector_copy(dir, n, 3);
		vector_scale(dir, -1, 3);

		if (!raycast_triangle(eye, dir, v, &dist))
			continue;

		f32 push_vec[3];
		float push = fmaxf(HEIGHT - dist + 0.5f, 0);

		vector_copy(push_vec, n, 3);
		vector_scale(push_vec, push, 3);
		vector_add(p->pos, push_vec, p->pos, 3);
	}
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

	p->headbob_timer_last = p->headbob_timer;
	p->headbob_timer += vector_magnitude(p->vel, 3);

	if (signbit(cosf(p->headbob_timer)) !=
		signbit(cosf(p->headbob_timer_last)))
	{
		const f32 vol = sqrtf(velmagsqr) * 2;
		const u16 freq = CONF_AUDIO_FREQ + (rand() & 0xFFF);

		mixer_ch_set_freq(SFXC_PLAYER, freq);
		mixer_ch_set_vol(SFXC_PLAYER, vol, vol);
		wav64_play(&footstep2_sfx, SFXC_PLAYER);
	}

	if (iparms.held.l)
	{
		vector_add(p->pos, (f32[3]) {0, -0.1f, 0}, p->pos, 3);
	}
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

void player_update(const struct scene *s, struct player *p,
		   const struct input_parms iparms)
{
	vector_copy(p->view.angles_last, p->view.angles, 2);
	vector_copy(p->view.eye_last, p->view.eye, 3);
	vector_copy(p->turn_offset_last, p->turn_offset, 2);
	vector_copy(p->view.eye_last, p->pos, 3);
	vector_copy(p->vel_last, p->vel, 3);

	_player_camera_look_update(p, iparms);
	_player_friction(p);
	_player_accelerate(p, iparms);
	vector_add(p->pos, p->vel, p->pos, 3);

	static u8 gen_col_mesh = 0;
	static struct collision_mesh cm;

	if (!gen_col_mesh)
	{
		collision_mesh_gen(&cm, s->meshes + 3);
		gen_col_mesh = 1;
	}

	_player_floor_collision(&cm, p);
	vector_copy(p->view.eye, p->pos, 3);
}

void player_items_update(struct player *p, const struct input_parms iparms)
{
	/*
	 * Gun Fires
	 */
	p->recoil_amnt_last = p->recoil_amnt;

	if (iparms.press.z && p->item_selected == ITEM_SELECT_PISTOL)
	{
		scene_anims_set_flags(&p->pistol.s, ANIM_IS_PLAYING);
		scene_anims_set_frame(&p->pistol.s, 0);
		wav64_play(&pistol_fire_sfx, SFXC_ITEM);
		p->recoil_amnt += 0.2f;
		vector_copy(p->recoil_dir, (f32[2]) {
			(f32)((rand() % 255) - 127) / 128.0f,
			(f32)((rand() % 255) - 127) / 128.0f}, 2);
	}

	if (p->recoil_amnt > 0.0f)
	{
		p->recoil_amnt -= 0.35f * p->recoil_amnt;
		if (p->recoil_amnt < 0.001f)
			p->recoil_amnt = 0.0f;
	}

	scene_anims_update(&p->pistol.s);
}

void player_item_draw(const struct player *p, const f32 subtick)
{
	glLoadIdentity();
	glScalef(0.1f, 0.1f, 0.1f);
	glTranslatef(1.35f, -2.0f, -2.2f);

	const f32 velmag_a = vector_magnitude_sqr(p->vel_last, 3);
	const f32 velmag_b = vector_magnitude_sqr(p->vel, 3);
	const f32 velmag_lerp =
		sqrtf(lerpf(velmag_a, velmag_b, subtick)) * 0.5f;
	const f32 headbob_timer_lerp = lerpf(p->headbob_timer_last,
				p->headbob_timer, subtick);

	glTranslatef(sinf(headbob_timer_lerp) * velmag_lerp,
	      fabsf(cosf(headbob_timer_lerp) * velmag_lerp * 0.5f), 0);

	f32 turn_offset_lerp[2];

	vector_lerp(p->turn_offset_last, p->turn_offset,
	     subtick, turn_offset_lerp, 2);
	vector_scale(turn_offset_lerp, 0.35f, 2);

	glTranslatef(-turn_offset_lerp[0], -turn_offset_lerp[1], 0);
	glRotatef(-90, 0, 1, 0);
	glRotatef(turn_offset_lerp[0] * 30, 0, 1, 0);
	glRotatef(turn_offset_lerp[1] * 30, 0, 0, 1);

	switch (p->item_selected)
	{
	case ITEM_SELECT_PISTOL:
		scene_draw(&p->pistol.s, subtick);
		return;

	case ITEM_SELECT_BONG:
		return;

	default:
		return;
	}
}

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
