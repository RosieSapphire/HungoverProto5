#include <GL/gl.h>

#include "engine/vector.h"
#include "engine/util.h"
#include "engine/camera.h"
#include "engine/player.h"

#include "game/testroom.h"

static struct scene scene;
static struct camera cam;

static struct player player;

static enum testroom_flags testroom_flags;

/**
 * testroom_load - Loads assets for Testroom
 */
void testroom_load(void)
{
	camera_init(&cam);
	player_init(&player, ITEM_HAS_PISTOL);
	scene_read_file(&scene, "rom:/Test.scn");
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
		testroom_flags ^= TR_FREECAM_ENABLED;

	if (testroom_flags & TR_FREECAM_ENABLED)
		camera_update(&cam, iparms);
	else
		player_update(&player, iparms);

	scene_anims_update(&scene);
	player_items_update(&player, iparms);

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
	if (testroom_flags & TR_FREECAM_ENABLED)
		camera_view_matrix_setup(&cam, subtick);
	else
		player_camera_view_matrix_setup(&player, subtick);

	scene_draw(&scene, subtick);

	if (testroom_flags & TR_FREECAM_ENABLED)
	{
		glDisable(GL_DEPTH_TEST);
		return;
	}

	glLoadIdentity();
	glScalef(0.1f, 0.1f, 0.1f);
	glTranslatef(1.35f, -1.8f, -2.2f);
	glRotatef(-90, 0, 1, 0);
	player_item_draw(&player, subtick);

	glDisable(GL_DEPTH_TEST);
}
