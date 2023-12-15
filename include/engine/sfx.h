/**
 * @file include/engine/sfx.h
 */

#ifndef _ENGINE_SFX_H_
#define _ENGINE_SFX_H_

#include <libdragon.h>

/**
 * Sound Effect Channels
 */
enum sfx_channels
{
	SFXC_MUSIC0, ///< Music channel 0
	SFXC_MUSIC1, ///< Music channel 1
	SFXC_MUSIC2, ///< Music channel 2
	SFXC_ITEM0, ///< Item channel 0
	SFXC_ITEM1, ///< Item channel 1
	SFXC_PLAYER, ///< Player channel
	SFXC_FOOTSTEPS, ///< Footsteps channel
	SFXC_COUNT, ///< SFX channel count
};

extern wav64_t title_music_intro;
extern wav64_t title_music_init;
extern wav64_t title_music_main;
extern wav64_t pistol_fire_sfx;
extern wav64_t pistol_click_sfx;
extern wav64_t pistol_pullout_sfx;
extern wav64_t footstep2_sfx;
extern wav64_t bong_pullout_sfx;
extern wav64_t bong_bubbling_sfx;
extern wav64_t lighter_flick_sfx;
extern wav64_t bong_hit_good_sfx;
extern wav64_t bong_hit_bad_sfx;
extern wav64_t cough_heavy0_sfx;
extern wav64_t cough_heavy1_sfx;
extern wav64_t cough_heavy2_sfx;
extern wav64_t cough_medium0_sfx;
extern wav64_t cough_medium1_sfx;
extern wav64_t cough_medium2_sfx;
extern wav64_t cough_small0_sfx;
extern wav64_t cough_small1_sfx;
extern wav64_t cough_small2_sfx;
extern wav64_t trip_music;
extern wav64_t nitrous_whine;

void sfx_load(void);

#endif /* _ENGINE_SFX_H_ */
