#include "game/testroom.h"

void testroom_load(void)
{

}

void testroom_unload(void)
{

}

enum scene_index testroom_update(struct input_parms iparms)
{
	if (iparms.press.start)
		return (SCENE_TITLE);

	return (SCENE_TESTROOM);
}

void testroom_draw(__attribute__((unused))f32 subtick)
{

}
