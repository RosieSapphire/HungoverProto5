#ifndef _ENGINE_SFX_H_
#define _ENGINE_SFX_H_

#include <libdragon.h>

/**
 * enum sfx_channels - Sound Effect Channels
 * @SFXC_MUSIC: Music Channel
 * @NUM_SFXC: Number of Channels
 */
enum sfx_channels
{
	SFXC_MUSIC,
	SFXC_ITEM,
	NUM_SFXC,
};

extern wav64_t title_music, pistol_fire_sfx;

void sfx_load(void);

#endif /* _ENGINE_SFX_H_ */
