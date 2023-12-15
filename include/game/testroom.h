/**
 * @file include/game/testroom.h
 */

#ifndef _GAME_TESTROOM_H_
#define _GAME_TESTROOM_H_

#include "engine/types.h"
#include "engine/input.h"
#include "engine/scene.h"

/**
 * Testroom Flags
 */
enum testroom_flags
{
	TR_FREECAM_ENABLED = 0x1, ///<  Whether or not Freecam is Enabled
};

void testroom_load(void);
void testroom_unload(void);
enum scene_index testroom_update(struct input_parms iparms);
void testroom_draw(f32 subtick);

#endif /* _GAME_TESTROOM_H_ */
