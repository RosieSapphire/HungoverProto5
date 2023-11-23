#include <GL/gl.h>

#include "engine/sfx.h"
#include "engine/util.h"
#include "engine/vector.h"
#include "engine/player.h"

static struct collision_mesh floormesh;

void player_init(const struct scene *s, struct player *p,
		 u8 items_equipped_flags)
{
	camera_init(&p->view);
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

	collision_mesh_gen(&floormesh, s->meshes + 3);
}

void player_terminate(struct player *p)
{
	p->item_flags = ITEM_HAS_NONE;
	scene_destroy(&p->pistol.s);
}

void player_update(struct player *p, const struct input_parms iparms)
{
	vector_copy(p->view.angles_last, p->view.angles, 2);
	vector_copy(p->view.eye_last, p->view.eye, 3);
	vector_copy(p->turn_offset_last, p->turn_offset, 2);
	vector_copy(p->view.eye_last, p->pos, 3);
	vector_copy(p->vel_last, p->vel, 3);

	player_camera_look_update(p, iparms);
	player_friction(p);
	player_accelerate(p, iparms);
	vector_add(p->pos, p->vel, p->pos, 3);


	player_floor_collision(&floormesh, p);
	vector_copy(p->view.eye, p->pos, 3);
}

void player_items_update(struct player *p, const struct input_parms iparms)
{
	/*
	 * Gun Fires
	 */
	p->recoil_amnt_last = p->recoil_amnt;

	switch (p->item_selected)
	{
	case ITEM_SELECT_PISTOL:
		if (iparms.press.z)
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

	default:
		break;
	}
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
