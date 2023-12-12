#ifndef _ENGINE_SFX_H_
#define _ENGINE_SFX_H_

#include <libdragon.h>

/**
 * enum sfx_channels - Sound Effect Channels
 * @SFXC_MUSIC0: Music Channel 0
 * @SFXC_MUSIC1: Music Channel 1
 * @SFXC_MUSIC2: Music Channel 2
 * @SFXC_ITEM0: Item Channel 0
 * @SFXC_ITEM1: Item Channel 1
 * @SFXC_PLAYER: Player Channel
 * @SFXC_FOOTSTEPS: Footsteps Channel
 * @SFXC_COUNT: Number of Channels
 */
enum sfx_channels
{
	SFXC_MUSIC0,
	SFXC_MUSIC1,
	SFXC_MUSIC2,
	SFXC_ITEM0,
	SFXC_ITEM1,
	SFXC_PLAYER,
	SFXC_FOOTSTEPS,
	SFXC_COUNT,
};

extern wav64_t title_music_intro, title_music_init, title_music_main,
	pistol_fire_sfx, pistol_click_sfx, pistol_pullout_sfx,
	footstep2_sfx, bong_pullout_sfx, bong_bubbling_sfx, lighter_flick_sfx,
	bong_hit_good_sfx, bong_hit_bad_sfx,
	cough_heavy0_sfx, cough_heavy1_sfx, cough_heavy2_sfx,
	cough_medium0_sfx, cough_medium1_sfx, cough_medium2_sfx,
	cough_small0_sfx, cough_small1_sfx, cough_small2_sfx,
	trip_music, nitrous_whine;

void sfx_load(void);

#endif /* _ENGINE_SFX_H_ */
