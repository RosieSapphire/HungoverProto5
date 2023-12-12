#ifndef _GAME_TESTROOM_H_
#define _GAME_TESTROOM_H_

#include "engine/types.h"
#include "engine/input.h"
#include "engine/scene.h"

/**
 * enum testroom_flags - Testroom Flags
 * @TR_FREECAM_ENABLED: Whether Freecam is Enabled (Toggled by Start)
 */
enum testroom_flags
{
	TR_FREECAM_ENABLED = 0x1,
};

void testroom_load(void);
void testroom_unload(void);
enum scene_index testroom_update(struct input_parms iparms);
void testroom_draw(f32 subtick);

#endif /* _GAME_TESTROOM_H_ */
