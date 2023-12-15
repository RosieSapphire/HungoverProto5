#include <GL/gl.h>

#include "engine/sfx.h"
#include "engine/util.h"
#include "engine/vector.h"
#include "engine/player.h"

static const s8 item_has_flags[ITEM_COUNT] = {
	ITEM_HAS_PISTOL,
	ITEM_HAS_BONG,
};

static const s8 item_select[ITEM_COUNT] = {
	ITEM_SELECT_PISTOL,
	ITEM_SELECT_BONG,
};

static const char *item_paths[ITEM_COUNT] = {
	"rom:/Pistol.scn",
	"rom:/Bong.scn",
};

static void (*item_use_funcs[ITEM_COUNT])(struct player *,
					  const struct input_parms) = {
	player_pistol_check_use,
	player_bong_check_use,
};

/**
 * Loads player's Item.
 * @param[in,out] p Player whos item needs to be loaded.
 * @param[in] flags_last Last state of Player Has Flags.
 */
void player_item_load(struct player *p, const u8 flags_last)
{
	if (p->item_flags == ITEM_HAS_NONE)
		return;

	for (u8 i = 0; i < ITEM_COUNT; i++)
	{
		if ((p->item_flags ^ flags_last) != item_has_flags[i])
			continue;

		struct scene *s = &p->items[i].s;

		scene_read_file(s, item_paths[i]);
		scene_anims_set_flags(s, ANIM_IS_PLAYING);
		scene_anims_set_frame(s, 0);
		p->item_selected = item_select[i];
	}

	struct item *item = p->items + p->item_selected;

	switch (p->item_selected)
	{
	case ITEM_SELECT_PISTOL:
		item->qty1 = 17;
		item->qty2 = 17 * 2;
		break;

	case ITEM_SELECT_BONG:
		item->qty1 = 3;
		item->qty2 = 0;
		break;

	default:
		break;
	};
}

/**
 * player_item_check_pickup - Checks if Player is Picking Up an Item
 * @param[in] s Scene to Check for Item
 * @param[in,out] p Player Structure
 */
void player_item_check_pickup(struct scene *s, struct player *p)
{
	for (u16 i = 0; i < s->num_meshes; i++)
	{
		struct mesh *m = s->meshes + i;

		if (strncmp("PU.", m->name, 3) || !(m->flags & MESH_IS_ACTIVE))
			continue;

		f32 vec[3];
		const struct node *n = node_from_mesh_ind(&s->root_node, i);
		const f32 dist_touch = 2.4f;
		const u8 item_flags_last = p->item_flags;

		vector_sub(p->pos, n->trans[3], vec, 3);
		if (vector_magnitude_sqr(vec, 3) < dist_touch * dist_touch)
		{
			if (!strcmp(m->name + 3, "Pistol"))
			{
				p->item_flags |= ITEM_HAS_PISTOL;
				mixer_ch_set_vol(SFXC_ITEM0, 0.3f, 0.4f);
				wav64_play(&pistol_pullout_sfx, SFXC_ITEM0);
			}
			else if (!strcmp(m->name + 3, "Bong"))
			{
				p->item_flags |= ITEM_HAS_BONG;
				mixer_ch_set_vol(SFXC_ITEM0, 0.6f, 0.7f);
				wav64_play(&bong_pullout_sfx, SFXC_ITEM0);
			}

			player_item_load(p, item_flags_last);
			m->flags &= ~(MESH_IS_ACTIVE);
		}
	}
}

static void _player_item_check_switching(struct player *p,
					 const struct input_parms iparms)
{
	const u8 num_items = __builtin_popcount(p->item_flags);
	const s8 item_selected_last = p->item_selected;
	struct item *item;

	p->item_selected += iparms.press.a - iparms.press.b;
	if (p->item_selected != item_selected_last)
	{
		if (p->item_selected >= num_items)
			p->item_selected = num_items - 1;
		if (p->item_selected < -1)
			p->item_selected = -1;
		if (!(p->item_flags & (1 << p->item_selected)) &&
		    p->item_selected != -1)
			p->item_selected = item_selected_last;
	}

	if (p->item_selected != item_selected_last)
	{
		switch (p->item_selected)
		{
		case ITEM_SELECT_PISTOL:
			item = p->items + 0;
			item->anim_index = 0;
			scene_anims_set_frame(&item->s, 0);
			scene_anims_set_flags(&item->s, ANIM_IS_PLAYING);
			mixer_ch_set_vol(SFXC_ITEM0, 0.3f, 0.4f);
			wav64_play(&pistol_pullout_sfx, SFXC_ITEM0);
			break;
		case ITEM_SELECT_BONG:
			item = p->items + 1;
			item->anim_index = 0;
			scene_anims_set_frame(&item->s, 0);
			scene_anims_set_flags(&item->s, ANIM_IS_PLAYING);
			mixer_ch_set_vol(SFXC_ITEM0, 0.6f, 0.7f);
			wav64_play(&bong_pullout_sfx, SFXC_ITEM0);
			break;
		default:
			break;
		}
	}
}

/**
 * Updates all attributes of players items
 * @param[in,out] p Player Structure
 * @param[in] iparms Input Parameters
 */
void player_items_update(struct player *p, const struct input_parms iparms)
{
	p->recoil_amnt_last = p->recoil_amnt;

	_player_item_check_switching(p, iparms);

	/*
	 * Check item Usage
	 */
	if (p->item_selected != ITEM_SELECT_NONE)
	{
		(*item_use_funcs[p->item_selected])(p, iparms);
		item_anim_update(p->items + p->item_selected);
	}

	/*
	 * Handling Recoil
	 */
	if (p->recoil_amnt <= 0.0f)
		return;

	p->recoil_amnt -= 0.35f * p->recoil_amnt;
	if (p->recoil_amnt < 0.001f)
		p->recoil_amnt = 0.0f;
}

/**
 * Draws the item currently in players hand
 * @param[in] p Player Structure
 * @param[in] subtick Subtick Between Frames
 */
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
	f32 turn_offset_lerp[2];

	glTranslatef(sinf(headbob_timer_lerp) * velmag_lerp,
		     fabsf(cosf(headbob_timer_lerp) * velmag_lerp * 0.5f), 0);
	vector_lerp(p->turn_offset_last, p->turn_offset,
		    subtick, turn_offset_lerp, 2);
	vector_scale(turn_offset_lerp, 0.35f, 2);

	glTranslatef(-turn_offset_lerp[0], -turn_offset_lerp[1], 0);
	glRotatef(-90, 0, 1, 0);
	glRotatef(turn_offset_lerp[0] * 30, 0, 1, 0);
	glRotatef(turn_offset_lerp[1] * 30, 0, 0, 1);
	const struct item *item = p->items + p->item_selected;

	item_node_draw(item, &item->s.root_node, subtick);
}
