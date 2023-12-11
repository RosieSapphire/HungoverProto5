#include "engine/sfx.h"
#include "engine/util.h"
#include "engine/vector.h"
#include "engine/player.h"

static wav64_t *cough_sfx[9] = {
	&cough_small0_sfx, &cough_small1_sfx, &cough_small2_sfx,
	&cough_medium0_sfx, &cough_medium1_sfx, &cough_medium2_sfx,
	&cough_heavy0_sfx, &cough_heavy1_sfx, &cough_heavy2_sfx,
};

void player_weed_update(struct player *p, const u16 num_coughs_max)
{
	static u16 cough_timer;
	f32 cough_percent = 0.0f;
	struct item *bong = p->items + 1;

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

void player_weed_cough_update(struct player *p,
			      const struct input_parms iparms,
			      u16 *num_coughs_max)
{
	struct item *bong = p->items + 1;
	const u8 is_coughing = bong->qty2 > 0;
	const u8 must_stop_smoking = (bong->usage_timer >= 56) || iparms.up.z;
	u16 cough_rand = 0;

	if (!must_stop_smoking || is_coughing)
		return;

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

		p->weed_high_amnt = 1.0f;
		p->weed_progress = 0;
		p->weed_duration = 240;
		mixer_ch_set_vol(SFXC_MUSIC0, 0, 0);
		mixer_ch_set_freq(SFXC_MUSIC0, 22050);
		wav64_play(&nitrous_whine, SFXC_MUSIC0);
	}
	bong->qty2 = cough_rand;
	*num_coughs_max = bong->qty2;
}

void player_weed_effect_draw(const struct player *p, const surface_t *surf,
			     const u32 tick_cnt, const u32 tick_cnt_last,
			     const f32 subtick)
{
	const f32 tick_cnt_lerp = lerpf(tick_cnt_last, tick_cnt, subtick);
	const f32 intensity = clampf((f32)p->weed_progress /
			      ((f32)p->weed_duration * 0.25f), 0, 1);
	const f32 scale = lerpf(1.0f, 1.04f, intensity);

	rdpq_set_mode_standard();
	rdpq_set_fog_color(RGBA32(0, 0, 0, intensity * 255));
	rdpq_mode_blender(RDPQ_BLENDER_ADDITIVE);
	rdpq_tex_blit(surf,
	       (CONF_WIDTH >> 1) + sinf(tick_cnt_lerp * 0.1f) * intensity * 4,
	       (CONF_HEIGHT >> 1) + cosf(tick_cnt_lerp * 0.1f) * intensity * 2,
	       &(const rdpq_blitparms_t) {
		.cx = (CONF_WIDTH >> 1),
		.cy = (CONF_HEIGHT >> 1),
		.scale_x = scale,
		.scale_y = scale,
	});
}
