#ifndef _GAME_TITLE_H_
#define _GAME_TITLE_H_

#include "engine/input.h"
#include "engine/types.h"

/**
 * enum title_music_state - Music State of Title
 * @TM_INTRO: Intro State
 * @TM_INIT: Initial State
 * @TM_MAIN: Main State
 * @TM_COUNT: Number of Title Music States
 */
enum title_music_state
{
	TM_INTRO = 0,
	TM_INIT,
	TM_MAIN,
	TM_COUNT,
};

void title_load(void);
void title_unload(void);
enum scene_index title_update(struct input_parms iparms);
void title_draw(f32 subtick);

#endif /* _GAME_TITLE_H_ */
