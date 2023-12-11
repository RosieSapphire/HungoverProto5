#include "engine/sfx.h"
#include "engine/vector.h"

#include "engine/player.h"

void player_pistol_check_use(struct player *p, const struct input_parms iparms)
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
