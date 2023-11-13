#include "engine/config.h"
#include "engine/sfx.h"

wav64_t title_music;

/**
 * sfx_load - Loads Sound Effects
 */
void sfx_load(void)
{
	audio_init(CONF_AUDIO_FREQ, CONF_NUM_AUDIO_BUF);
	mixer_init(NUM_SFXC);

	wav64_open(&title_music, "rom:/title_main.wav64");
	wav64_set_loop(&title_music, 1);
}
