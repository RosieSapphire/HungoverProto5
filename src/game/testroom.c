#include <GL/gl.h>

#include "engine/util.h"

#include "game/testroom.h"

static struct scene scene;
static f32 angle_last, angle;

/**
 * testroom_load - Loads assets for Testroom
 */
void testroom_load(void)
{
	scene_read_file(&scene, "rom:/Test.scn");
	angle_last = angle = 0.0f;
}

/**
 * testroom_unload - Unloads assets for Testroom
 */
void testroom_unload(void)
{
	scene_destroy(&scene);
}

/**
 * testroom_update - Updates Testroom and all its stuff
 * @iparms: Input Parameters
 *
 * Return: Desired Next Scene Index
 */
enum scene_index testroom_update(struct input_parms iparms)
{
	if (iparms.press.start)
		return (SCENE_TITLE);

	angle_last = angle;
	angle += 2.4f;

	return (SCENE_TESTROOM);
}

/**
 * testroom_draw - Draws all the stuff for Testroom
 * @subtick: Delta Value Between Frames
 */
void testroom_draw(f32 subtick)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	f32 angle_lerp = lerpf(angle_last, angle, subtick);

	glTranslatef(0, 0, -3);
	glRotatef(angle_lerp, 0, 1, 0);

	scene_draw(&scene);
	glDisable(GL_DEPTH_TEST);
}
