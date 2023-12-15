#include "engine/sfx.h"
#include "engine/vector.h"
#include "engine/player.h"

/**
 * Checks if player is using pistol.
 * @param[in,out] p Player
 * @param[in] iparms Input Parameters
 */
void player_pistol_check_use(struct player *p,
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
