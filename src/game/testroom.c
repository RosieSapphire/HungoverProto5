#include <GL/gl.h>

#include "engine/vector.h"
#include "engine/util.h"
#include "engine/camera.h"

#include "game/testroom.h"

static struct scene scene;
static struct scene pistol;
static struct camera cam;

/**
 * testroom_load - Loads assets for Testroom
 */
void testroom_load(void)
{
	camera_init(&cam);
	scene_read_file(&scene, "rom:/Test.scn");
	scene_read_file(&pistol, "rom:/Pistol.scn");

	scene_anims_set_flags(&pistol, ANIM_LOOPS);
}

/**
 * testroom_unload - Unloads assets for Testroom
 */
void testroom_unload(void)
{
	scene_destroy(&pistol);
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

	camera_update(&cam, iparms);
	scene_anims_update(&scene);
	scene_anims_update(&pistol);

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
	camera_view_matrix_setup(&cam, subtick);
	scene_draw(&scene, subtick);

	glLoadIdentity();
	glScalef(0.1f, 0.1f, 0.1f);
	glTranslatef(1.35f, -1.8f, -2.2f);
	glRotatef(-90, 0, 1, 0);
	scene_draw(&pistol, subtick);
	glDisable(GL_DEPTH_TEST);
}
