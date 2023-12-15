/**
 * @file src/engine/sfx.c
 */

#include "engine/config.h"
#include "engine/sfx.h"

wav64_t title_music_intro; ///< Title music intro
wav64_t title_music_init; ///< Title music init
wav64_t title_music_main; ///< Title music main
wav64_t pistol_fire_sfx; ///< Pistol fire sfx
wav64_t pistol_click_sfx; ///< Pistol click sfx
wav64_t pistol_pullout_sfx; ///< Pistol pullout sfx
wav64_t footstep2_sfx; ///< Footstep sfx
wav64_t bong_pullout_sfx; ///< Bong pullout sfx
wav64_t bong_bubbling_sfx; ///< Bong bubbling sfx
wav64_t lighter_flick_sfx; ///< Lighter flick sfx
wav64_t bong_hit_good_sfx; ///< Bong hit good sfx
wav64_t bong_hit_bad_sfx; ///< Bong hit bad sfx
wav64_t cough_heavy0_sfx; ///< Cough heavy 0
wav64_t cough_heavy1_sfx; ///< Cough heavy 1
wav64_t cough_heavy2_sfx; ///< Cough heavy 2
wav64_t cough_medium0_sfx; ///< Cough medium 0
wav64_t cough_medium1_sfx; ///< Cough medium 1
wav64_t cough_medium2_sfx; ///< Cough medium 2
wav64_t cough_small0_sfx; ///< Cough small 0
wav64_t cough_small1_sfx; ///< Cough small 1
wav64_t cough_small2_sfx; ///< Cough small 2
wav64_t trip_music; ///< Tripping music
wav64_t nitrous_whine; ///< Nitrous Oxide whine

static void _sfx_set_mixer_ch_vols(void)
{
	mixer_ch_set_vol(SFXC_MUSIC0, 0.8f, 0.8f);
	mixer_ch_set_vol(SFXC_MUSIC1, 0.8f, 0.8f);
	mixer_ch_set_vol(SFXC_MUSIC2, 0.8f, 0.8f);
	mixer_ch_set_vol(SFXC_ITEM0, 0.8f, 0.8f);
	mixer_ch_set_vol(SFXC_ITEM1, 0.8f, 0.8f);
	mixer_ch_set_vol(SFXC_PLAYER, 0.8f, 0.8f);
	mixer_ch_set_vol(SFXC_FOOTSTEPS, 0.8f, 0.8f);
}

static void _sfx_samples_open(void)
{
	wav64_open(&title_music_intro, "rom:/title_intro.wav64");
	wav64_open(&title_music_init, "rom:/title_init.wav64");
	wav64_open(&title_music_main, "rom:/title_main.wav64");
	wav64_open(&pistol_fire_sfx, "rom:/pistol_fire.wav64");
	wav64_open(&pistol_click_sfx, "rom:/pistol_click.wav64");
	wav64_open(&pistol_pullout_sfx, "rom:/pistol_pullout.wav64");
	wav64_open(&footstep2_sfx, "rom:/footstep2.wav64");
	wav64_open(&bong_pullout_sfx, "rom:/bong_pullout.wav64");
	wav64_open(&bong_bubbling_sfx, "rom:/bong_bubbling.wav64");
	wav64_open(&lighter_flick_sfx, "rom:/lighter_flick.wav64");
	wav64_open(&bong_hit_good_sfx, "rom:/bong_hit_good.wav64");
	wav64_open(&bong_hit_bad_sfx, "rom:/bong_hit_bad.wav64");
	wav64_open(&cough_heavy0_sfx, "rom:/cough_heavy0.wav64");
	wav64_open(&cough_heavy1_sfx, "rom:/cough_heavy1.wav64");
	wav64_open(&cough_heavy2_sfx, "rom:/cough_heavy2.wav64");
	wav64_open(&cough_medium0_sfx, "rom:/cough_medium0.wav64");
	wav64_open(&cough_medium1_sfx, "rom:/cough_medium1.wav64");
	wav64_open(&cough_medium2_sfx, "rom:/cough_medium2.wav64");
	wav64_open(&cough_small0_sfx, "rom:/cough_small0.wav64");
	wav64_open(&cough_small1_sfx, "rom:/cough_small1.wav64");
	wav64_open(&cough_small2_sfx, "rom:/cough_small2.wav64");
	wav64_open(&trip_music, "rom:/tripmusic.wav64");
	wav64_open(&nitrous_whine, "rom:/nitrous_whine.wav64");
}

static void _sfx_set_looping(void)
{
	wav64_set_loop(&title_music_intro, 0);
	wav64_set_loop(&title_music_init, 1);
	wav64_set_loop(&title_music_main, 1);
	wav64_set_loop(&pistol_fire_sfx, 0);
	wav64_set_loop(&pistol_click_sfx, 0);
	wav64_set_loop(&pistol_pullout_sfx, 0);
	wav64_set_loop(&footstep2_sfx, 0);
	wav64_set_loop(&bong_pullout_sfx, 0);
	wav64_set_loop(&bong_bubbling_sfx, 0);
	wav64_set_loop(&lighter_flick_sfx, 0);
	wav64_set_loop(&bong_hit_good_sfx, 0);
	wav64_set_loop(&bong_hit_bad_sfx, 0);
	wav64_set_loop(&cough_heavy0_sfx, 0);
	wav64_set_loop(&cough_heavy1_sfx, 0);
	wav64_set_loop(&cough_heavy2_sfx, 0);
	wav64_set_loop(&cough_medium0_sfx, 0);
	wav64_set_loop(&cough_medium1_sfx, 0);
	wav64_set_loop(&cough_medium2_sfx, 0);
	wav64_set_loop(&cough_small0_sfx, 0);
	wav64_set_loop(&cough_small1_sfx, 0);
	wav64_set_loop(&cough_small2_sfx, 0);
	wav64_set_loop(&trip_music, 0);
	wav64_set_loop(&nitrous_whine, 1);
}

/**
 * Loads Sound Effects
 */
void sfx_load(void)
{
	audio_init(CONF_AUDIO_FREQ, CONF_NUM_AUDIO_BUF);
	mixer_init(SFXC_COUNT);

	_sfx_set_mixer_ch_vols();
	_sfx_samples_open();
	_sfx_set_looping();
}
