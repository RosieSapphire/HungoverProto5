#include "engine/config.h"
#include "engine/sfx.h"

wav64_t title_music, pistol_fire_sfx;

/**
 * sfx_load - Loads Sound Effects
 */
void sfx_load(void)
{
	audio_init(CONF_AUDIO_FREQ, CONF_NUM_AUDIO_BUF);
	mixer_init(NUM_SFXC);

	mixer_ch_set_vol(SFXC_MUSIC, 0.8f, 0.8f);
	mixer_ch_set_vol(SFXC_ITEM, 0.8f, 0.8f);

	wav64_open(&title_music, "rom:/title_main.wav64");
	wav64_open(&pistol_fire_sfx, "rom:/pistol_fire.wav64");

	wav64_set_loop(&title_music, 1);
	wav64_set_loop(&pistol_fire_sfx, 0);
}
