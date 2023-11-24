#include <GL/gl.h>

#include "engine/sfx.h"
#include "engine/util.h"
#include "engine/vector.h"
#include "engine/player.h"

void player_item_load(struct player *p)
{
	s8 item_has_flags[ITEM_COUNT] = {
		ITEM_HAS_PISTOL,
		ITEM_HAS_BONG,
	};

	s8 item_select[ITEM_COUNT] = {
		ITEM_SELECT_PISTOL,
		ITEM_SELECT_BONG,
	};

	const char *item_paths[ITEM_COUNT] = {
		"rom:/Pistol.scn",
		"rom:/Bong.scn",
	};

	for (u8 i = 0; i < ITEM_COUNT; i++)
	{
		if (p->item_flags == ITEM_HAS_NONE)
			continue;

		if (!(p->item_flags & item_has_flags[i]))
			continue;

		scene_read_file(&p->items[i].s, item_paths[i]);
		scene_anims_set_flags(&p->items[i].s, ANIM_LOOPS);
		scene_anims_set_frame(&p->items[i].s, 0);
		p->item_selected = item_select[i];
	}
}

void player_check_pickup(struct scene *s, struct player *p)
{
	for (u16 i = 0; i < s->num_meshes; i++)
	{
		struct mesh *m = s->meshes + i;

		if (strncmp("PU.", m->name, 3) || !(m->flags & MESH_IS_ACTIVE))
			continue;

		f32 vec[3];
		f32 dist;
		const struct node *n = node_from_mesh_ind(&s->root_node, i);

		vector_sub(p->pos, n->trans[3], vec, 3);
		dist = vector_magnitude_sqr(vec, 3);

		if (dist < 3.5f)
		{
			p->item_flags |= ITEM_HAS_PISTOL;
			p->item_selected = ITEM_SELECT_PISTOL;
			player_item_load(p);
			m->flags &= ~(MESH_IS_ACTIVE);
		}
	}
}

void player_items_update(struct player *p, const struct input_parms iparms)
{
	p->recoil_amnt_last = p->recoil_amnt;

	switch (p->item_selected)
	{
	case ITEM_SELECT_PISTOL:
		if (iparms.press.z)
		{
			scene_anims_set_flags(&p->items[0].s, ANIM_IS_PLAYING);
			scene_anims_set_frame(&p->items[0].s, 0);
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

		scene_anims_update(&p->items[0].s);

	default:
		break;
	}
}

void player_item_draw(const struct player *p, const f32 subtick)
{
	if (p->item_selected == ITEM_SELECT_NONE)
		return;

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
		scene_draw(&p->items[0].s, subtick);
		return;

	case ITEM_SELECT_BONG:
		return;

	default:
		return;
	}
}
