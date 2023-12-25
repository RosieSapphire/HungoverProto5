#include "engine/sfx.h"
#include "engine/util.h"
#include "engine/vector.h"
#include "engine/player.h"

static wav64_t *cough_sfx[9] = {
	&cough_small0_sfx, &cough_small1_sfx, &cough_small2_sfx,
	&cough_medium0_sfx, &cough_medium1_sfx, &cough_medium2_sfx,
	&cough_heavy0_sfx, &cough_heavy1_sfx, &cough_heavy2_sfx,
};

/**
 * Check if player is using bong
 * @param[in,out] p Player
 * @param[in] iparms Input Parameters
 */
void player_bong_check_use(struct player *p, const struct input_parms iparms)
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
		    bong->usage_timer < BONG_USAGE_TIMER_MAX &&
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

/**
 * Setup coughing from smoking weed
 * @param[in,out] p Player
 * @return Number of max coughs
 */
u16 player_bong_cough_setup(struct player *p)
{
	struct item *bong = p->items + 1;
	u16 cough_rand = 0;

	if (bong->usage_timer_last)
	{
		f32 usage_timer_exp = ((f32)bong->usage_timer_last / 56.0f);

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

		p->drug_progress = 0;
		p->drug_duration = 240;
		mixer_ch_set_vol(SFXC_MUSIC0, 0, 0);
		mixer_ch_set_freq(SFXC_MUSIC0, 22050);
		/*
		 * TODO: Wrong SFX
		 */
		wav64_play(&n2o_whine_sfx, SFXC_MUSIC0);
	}
	bong->qty2 = cough_rand;
	return (bong->qty2);
}

/**
 * Update weed high effect for player
 * @param[in,out] p Player
 * @param[in] num_coughs_max Maximum number of coughs this session
 */
void player_bong_weed_effect_update(struct player *p,
				    const u16 num_coughs_max)
{
	static u16 cough_timer;
	f32 cough_percent = 0.0f;
	struct item *bong = p->items + 1;

	if (p->drug_duration)
	{
		const f32 trip_vol = player_drug_get_intensity(p);

		mixer_ch_set_vol(SFXC_MUSIC0, trip_vol, trip_vol);
		p->drug_progress++;
	}

	if (num_coughs_max)
		cough_percent = (f32)bong->qty2 / (f32)num_coughs_max;

	if (cough_timer)
		cough_timer--;

	if (bong->qty2 && !cough_timer)
	{
		const u16 cough_freq = CONF_AUDIO_FREQ + (rand() & 0xFFF);
		const u16 cough_col = fmin((u16)(cough_percent * 3), 2);
		const u16 cough_row = rand() % 3;

		bong->qty2--;
		cough_timer = 12 + (rand() % 16);
		p->recoil_dir[0] = ((f32)(rand() % 512) / 512.0f) - 0.5f;
		p->recoil_dir[1] = -1;
		vector_scale(p->recoil_dir, cough_percent, 2);
		p->recoil_amnt = (f32)(rand() % 256) * 0.01f;

		mixer_ch_set_vol(SFXC_PLAYER, 0.5f, 0.5f);
		mixer_ch_set_freq(SFXC_PLAYER, cough_freq);
		wav64_play(cough_sfx[cough_col * 3 + cough_row], SFXC_PLAYER);
	}
}

/**
 * Draws the weed effect for player
 * @param[in] p Player
 * @param[in] surf Framebuffer surface
 * @param[in] tick_cnt Number of ticks passed current
 * @param[in] tick_cnt_last Number of ticks passed previous
 * @param[in] subtick Subtick between frames
 */
void player_bong_weed_effect_draw(const struct player *p,
				  const surface_t *surf, const u32 tick_cnt,
				  const u32 tick_cnt_last, const f32 subtick)
{
	const f32 tick_cnt_lerp = lerpf(tick_cnt_last, tick_cnt, subtick);
	const f32 intensity = player_drug_get_intensity(p);
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
