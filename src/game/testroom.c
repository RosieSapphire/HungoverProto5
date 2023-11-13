#include <GL/gl.h>

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
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
