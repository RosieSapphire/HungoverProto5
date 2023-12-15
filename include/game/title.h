/**
 * @file include/game/title.h
 */

#ifndef _GAME_TITLE_H_
#define _GAME_TITLE_H_

#include "engine/input.h"
#include "engine/types.h"

/**
 * Music State of Title
 */
enum title_music_state
{
	TM_INTRO = 0, ///< Intro State
	TM_INIT, ///< Initial State
	TM_MAIN, ///< Main State
	TM_COUNT, ///< Number of Title Music States
};

void title_load(void);
void title_unload(void);
enum scene_index title_update(struct input_parms iparms);
void title_draw(f32 subtick);

#endif /* _GAME_TITLE_H_ */
