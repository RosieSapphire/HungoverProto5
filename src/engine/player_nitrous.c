#include "engine/sfx.h"
#include "engine/util.h"
#include "engine/vector.h"
#include "engine/player.h"

/**
 * Checks if the player is using Nitrous Oxide
 * @param[in] p Player to Check
 * @param[in] iparms Input Parameters
 */
void player_n2o_check_use(struct player *p, const struct input_parms iparms)
{
	struct item *n2o = p->items + 1;

	n2o->usage_timer_last = n2o->usage_timer;

	if (iparms.press.z && item_anim_at_end(n2o, 0) &&
	    n2o->qty2 == 0)
	{
		n2o->usage_timer = 1;
		n2o->anim_index = 1;
		item_anim_set_frame(n2o, 1, 0);
		mixer_ch_set_vol(SFXC_ITEM1, 0.6f, 0.6f);
		wav64_play(&lighter_flick_sfx, SFXC_ITEM1);
	}

	if (n2o->anim_index == 1)
	{
		if ((iparms.held.z || iparms.press.z) &&
		    n2o->usage_timer < 56 &&
		    n2o->usage_timer != 0)
		{
			item_anim_set_flags(n2o, 1, ANIM_IS_PLAYING);
			mixer_ch_set_freq(SFXC_ITEM0,
					  32000 + (n2o->usage_timer * 200));
			n2o->usage_timer += n2o->usage_timer > 0;
		}
		else
		{
			mixer_ch_set_freq(SFXC_ITEM0, 32000);
			mixer_ch_stop(SFXC_ITEM0);
			item_anim_set_flags(n2o, 1,
					    ANIM_IS_PLAYING | ANIM_IS_BACKWARD);
			n2o->usage_timer = 0;
		}
	}

	if (n2o->usage_timer_last == 17 && n2o->usage_timer == 18)
	{
		mixer_ch_set_vol(SFXC_ITEM0, 0.4f, 0.4f);
		
		// FIXME: Change this to Nitrous Huffing
		wav64_play(&bong_bubbling_sfx, SFXC_ITEM0);
	}
}

void player_n2o_trip_setup(struct player *p)
{
	struct item *n2o = p->items + 1;

	if (n2o->usage_timer_last)
	{
		if (n2o->usage_timer_last >= 48 && n2o->usage_timer_last <= 50)
		{
			mixer_ch_set_vol(SFXC_ITEM1, 0.3f, 0.3f);
			wav64_play(&bong_hit_good_sfx, SFXC_ITEM1);
		}
		else
		{
			mixer_ch_set_vol(SFXC_ITEM1, 0.3f, 0.3f);
			wav64_play(&bong_hit_bad_sfx, SFXC_ITEM1);
		}

		p->which_drug = ON_DRUG_NITROUS;
		p->drug_high_amnt = 1.0f;
		p->drug_progress = 0;
		p->drug_duration = 240;
		mixer_ch_set_vol(SFXC_MUSIC0, 0, 0);
		mixer_ch_set_freq(SFXC_MUSIC0, 22050);
		wav64_play(&nitrous_whine, SFXC_MUSIC0);
	}
}

/**
 * player_n2o_effect_update - Updates the Weed Effect for Player
 * @p: Player to Update for
 */
void player_n2o_effect_update(struct player *p)
{
	if (!p->drug_duration)
		return;

	/*
	 * TODO: Make this fade out at end
	 */
	const f32 t = ((f32)p->drug_progress / (f32)p->drug_duration) *
		p->drug_high_amnt;
	f32 womp_vol = lerpf(0.0f, 0.5f, fminf(t, 1.0f));

	mixer_ch_set_vol(SFXC_MUSIC0, womp_vol, womp_vol);
	p->drug_progress++;
}

/**
 * Draws the nitrous oxide effect using framebuffer
 * @param[in] p Player to refer to
 * @param[in] surf Color framebuffer to overlay
 * @param[in] tick_cnt Current tick count
 * @param[in] tick_cnt_last Previous tick count
 * @param[in] subtick Subtick between frames
 */
void player_n2o_effect_draw(const struct player *p, const surface_t *surf,
			    const u32 tick_cnt, const u32 tick_cnt_last,
			    const f32 subtick)
{
	const f32 tick_cnt_lerp = lerpf(tick_cnt_last, tick_cnt, subtick);
	const f32 intensity = clampf((f32)p->drug_progress /
			      ((f32)p->drug_duration * 0.25f), 0, 1);
	const f32 scale = lerpf(1.0f, 1.04f, intensity);

	rdpq_set_mode_standard();
	rdpq_set_fog_color(RGBA32(0, 0, 0, intensity * 255));
	rdpq_mode_blender(RDPQ_BLENDER_ADDITIVE);
	rdpq_tex_blit(surf,
		      (CONF_WIDTH >> 1) + sinf(tick_cnt_lerp * 0.1f) *
		      intensity * 4,
		      (CONF_HEIGHT >> 1) + cosf(tick_cnt_lerp * 0.1f) *
		      intensity * 2,
		      &(const rdpq_blitparms_t) {
				.cx = (CONF_WIDTH >> 1),
				.cy = (CONF_HEIGHT >> 1),
				.scale_x = scale,
				.scale_y = scale,
			}
	);
}
