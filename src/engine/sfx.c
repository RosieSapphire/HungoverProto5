#include "engine/config.h"
#include "engine/sfx.h"

wav64_t title_music_intro, title_music_init, title_music_main,
	pistol_fire_sfx, pistol_click_sfx, pistol_pullout_sfx,
	footstep2_sfx, bong_pullout_sfx, bong_bubbling_sfx, lighter_flick_sfx,
	cough_heavy0_sfx, cough_heavy1_sfx, cough_heavy2_sfx,
	cough_medium0_sfx, cough_medium1_sfx, cough_medium2_sfx,
	cough_small0_sfx, cough_small1_sfx, cough_small2_sfx;

/**
 * sfx_load - Loads Sound Effects
 */
void sfx_load(void)
{
	audio_init(CONF_AUDIO_FREQ, CONF_NUM_AUDIO_BUF);
	mixer_init(NUM_SFXC);

	mixer_ch_set_vol(SFXC_MUSIC0, 0.8f, 0.8f);
	mixer_ch_set_vol(SFXC_MUSIC1, 0.8f, 0.8f);
	mixer_ch_set_vol(SFXC_MUSIC2, 0.8f, 0.8f);
	mixer_ch_set_vol(SFXC_ITEM0, 0.8f, 0.8f);
	mixer_ch_set_vol(SFXC_ITEM1, 0.8f, 0.8f);
	mixer_ch_set_vol(SFXC_PLAYER, 0.8f, 0.8f);

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

	wav64_open(&cough_heavy0_sfx, "rom:/cough_heavy0.wav64");
	wav64_open(&cough_heavy1_sfx, "rom:/cough_heavy1.wav64");
	wav64_open(&cough_heavy2_sfx, "rom:/cough_heavy2.wav64");
	wav64_open(&cough_medium0_sfx, "rom:/cough_medium0.wav64");
	wav64_open(&cough_medium1_sfx, "rom:/cough_medium1.wav64");
	wav64_open(&cough_medium2_sfx, "rom:/cough_medium2.wav64");
	wav64_open(&cough_small0_sfx, "rom:/cough_small0.wav64");
	wav64_open(&cough_small1_sfx, "rom:/cough_small1.wav64");
	wav64_open(&cough_small2_sfx, "rom:/cough_small2.wav64");

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
	wav64_set_loop(&cough_heavy0_sfx, 0);
	wav64_set_loop(&cough_heavy1_sfx, 0);
	wav64_set_loop(&cough_heavy2_sfx, 0);
	wav64_set_loop(&cough_medium0_sfx, 0);
	wav64_set_loop(&cough_medium1_sfx, 0);
	wav64_set_loop(&cough_medium2_sfx, 0);
	wav64_set_loop(&cough_small0_sfx, 0);
	wav64_set_loop(&cough_small1_sfx, 0);
	wav64_set_loop(&cough_small2_sfx, 0);
}
