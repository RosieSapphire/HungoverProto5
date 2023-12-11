#include <GL/gl.h>

#include "engine/sfx.h"
#include "engine/util.h"
#include "engine/vector.h"
#include "engine/item.h"
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

/**
 * player_item_load - Loads player's Item
 * @p: Player whos item needs to be loaded
 * @flags_last: Last state of Player Has Flags
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

void player_check_pickup(struct scene *s, struct player *p)
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
		if (!(p->item_flags & (1 << p->item_selected))
				&& p->item_selected != -1)
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

static void _player_pistol_check_use(struct player *p,
				     const struct input_parms iparms)
{
	struct item *pistol = p->items + 0;

	if (!iparms.press.z)
		return;

	if (!item_anim_at_end(pistol, 0) && !item_anim_is_index(pistol, 1))
		return;

	u8 has_loaded = pistol->qty1 > 0;
	u8 has_reserve = pistol->qty2 > 0;

	if (has_loaded)
	{
		pistol->anim_index = 1;
		pistol->qty1--;
		scene_anims_set_flags(&pistol->s, ANIM_IS_PLAYING);
		scene_anims_set_frame(&pistol->s, 0);
		mixer_ch_set_vol(SFXC_ITEM1, 0.8f, 0.8f);
		wav64_play(&pistol_fire_sfx, SFXC_ITEM1);
		p->recoil_amnt += 0.2f;
		vector_copy(p->recoil_dir, (f32[2]) {
			(f32)((rand() % 255) - 127) / 128.0f,
			(f32)((rand() % 255) - 127) / 128.0f},
			2);
	}
	else if (has_reserve && !has_loaded)
	{
		pistol->qty2 -= 17;
		pistol->qty1 += 17;
	}
	else if (!has_reserve && !has_loaded)
	{
		mixer_ch_set_vol(SFXC_ITEM0, 0.5f, 0.5f);
		wav64_play(&pistol_click_sfx, SFXC_ITEM0);
	}
}

static void _player_bong_check_use(struct player *p,
				   const struct input_parms iparms)
{
	struct item *bong = p->items + 1;

	bong->usage_timer_last = bong->usage_timer;

	if (iparms.press.z && item_anim_at_end(bong, 0) &&
		bong->qty2 == 0)
	{
		bong->usage_timer = 1;
		bong->anim_index = 1;
		item_anim_set_frame(bong, 1, 0);
		mixer_ch_set_vol(SFXC_ITEM1, 0.6f, 0.6f);
		wav64_play(&lighter_flick_sfx, SFXC_ITEM1);
	}

	if (bong->anim_index == 1)
	{
		if ((iparms.held.z || iparms.press.z) &&
				bong->usage_timer < 56 &&
				bong->usage_timer != 0)
		{
			item_anim_set_flags(bong, 1, ANIM_IS_PLAYING);
			mixer_ch_set_freq(SFXC_ITEM0,
		     		32000 + (bong->usage_timer * 200));
			bong->usage_timer += bong->usage_timer > 0;
		}
		else
		{
			mixer_ch_set_freq(SFXC_ITEM0, 32000);
			mixer_ch_stop(SFXC_ITEM0);
			item_anim_set_flags(bong, 1,
				ANIM_IS_PLAYING | ANIM_IS_BACKWARD);
			bong->usage_timer = 0;
		}
	}

	if (bong->usage_timer_last == 17 && bong->usage_timer == 18)
	{
		mixer_ch_set_vol(SFXC_ITEM0, 0.4f, 0.4f);
		wav64_play(&bong_bubbling_sfx, SFXC_ITEM0);
	}
}

void player_items_update(struct player *p, const struct input_parms iparms)
{
	p->recoil_amnt_last = p->recoil_amnt;

	_player_item_check_switching(p, iparms);

	/*
	 * Check item Usage
	 */
	static void (*item_use_funcs[ITEM_COUNT])(struct player *,
			      const struct input_parms) = {
		_player_pistol_check_use,
		_player_bong_check_use,
	};

	if (p->item_selected != ITEM_SELECT_NONE)
		(*item_use_funcs[p->item_selected])(p, iparms);

	/*
	 * Handling Coughing
	 */
	struct item *bong = p->items + 1;
	static u16 num_coughs_max;
	static u16 cough_timer;
	const u8 is_coughing = bong->qty2 > 0;
	const u8 must_stop_smoking = (bong->usage_timer >= 56) || iparms.up.z;

	if (must_stop_smoking && !is_coughing)
	{
		u16 cough_rand = 0;

		if (bong->usage_timer_last)
		{
			f32 usage_timer_exp =
				((f32)bong->usage_timer_last / 56.0f);

			usage_timer_exp *= usage_timer_exp * usage_timer_exp;
			usage_timer_exp *= 56;

			if ((u16)usage_timer_exp)
				cough_rand = (rand() % (u16)usage_timer_exp);

			if (bong->usage_timer_last >= 48 &&
			    bong->usage_timer_last <= 50)
			{
				mixer_ch_set_vol(SFXC_ITEM1, 0.3f, 0.3f);
				wav64_play(&bong_hit_good_sfx, SFXC_ITEM1);
				cough_rand = 0;
			}
			else
			{
				mixer_ch_set_vol(SFXC_ITEM1, 0.3f, 0.3f);
				wav64_play(&bong_hit_bad_sfx, SFXC_ITEM1);
			}

			p->weed_high_amnt = 1.0f;
			p->weed_progress = 0;
			p->weed_duration = 240;
			mixer_ch_set_vol(SFXC_MUSIC0, 0, 0);
			mixer_ch_set_freq(SFXC_MUSIC0, 22050);
			wav64_play(&nitrous_whine, SFXC_MUSIC0);
		}

		bong->qty2 = cough_rand;
		num_coughs_max = bong->qty2;
	}

	if (p->weed_duration)
	{
		/*
		 * TODO: Make this fade out at end
		 */
		const f32 t = ((f32)p->weed_progress / (f32)p->weed_duration) *
			p->weed_high_amnt;
		f32 trip_vol = lerpf(0.0f, 0.5f, fminf(t, 1.0f));

		mixer_ch_set_vol(SFXC_MUSIC0, trip_vol, trip_vol);
		p->weed_progress++;
	}

	f32 cough_percent = 0.0f;

	if (num_coughs_max)
		cough_percent = (f32)bong->qty2 / (f32)num_coughs_max;

	if (cough_timer)
		cough_timer--;

	if (bong->qty2 && !cough_timer)
	{
		const u16 cough_freq = CONF_AUDIO_FREQ + (rand() & 0xFFF);
		const u16 cough = rand() % 3;

		mixer_ch_set_vol(SFXC_PLAYER, 0.5f, 0.5f);
		mixer_ch_set_freq(SFXC_PLAYER, cough_freq);

		p->recoil_dir[0] = ((f32)(rand() % 512) / 512.0f) - 0.5f;
		p->recoil_dir[1] = -1;
		vector_scale(p->recoil_dir, cough_percent, 2);
		p->recoil_amnt = (f32)(rand() % 256) * 0.01f;
		if (cough_percent > 0.666f)
		{
			switch (cough)
			{
			case 0:
				wav64_play(&cough_heavy0_sfx,
					SFXC_PLAYER);
				break;

			case 1:
				wav64_play(&cough_heavy1_sfx,
					SFXC_PLAYER);
				break;

			case 2:
				wav64_play(&cough_heavy2_sfx,
					SFXC_PLAYER);
				break;

			default:
				exit(1);
			}
		}
		else if (cough_percent > 0.333f &&
				cough_percent < 0.666f)
		{
			switch (cough)
			{
			case 0:
				wav64_play(&cough_medium0_sfx,
					SFXC_PLAYER);
				break;

			case 1:
				wav64_play(&cough_medium1_sfx,
					SFXC_PLAYER);
				break;

			case 2:
				wav64_play(&cough_medium2_sfx,
					SFXC_PLAYER);
				break;

			default:
				exit(1);
			}
		}
		else
		{
			switch (cough)
			{
			case 0:
				wav64_play(&cough_small0_sfx,
					SFXC_PLAYER);
				break;

			case 1:
				wav64_play(&cough_small1_sfx,
					SFXC_PLAYER);
				break;

			case 2:
				wav64_play(&cough_small2_sfx,
					SFXC_PLAYER);
				break;

			default:
				exit(1);
			}
		}
		bong->qty2--;
		cough_timer = 12 + (rand() % 16);
	}


	if (p->item_selected != ITEM_SELECT_NONE)
		item_anim_update(p->items + p->item_selected);

	/*
	 * Handling Recoil
	 */
	if (p->recoil_amnt <= 0.0f)
		return;

	p->recoil_amnt -= 0.35f * p->recoil_amnt;
	if (p->recoil_amnt < 0.001f)
		p->recoil_amnt = 0.0f;
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
