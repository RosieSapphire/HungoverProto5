#include <GL/gl.h>

#include "engine/util.h"

#include "game/testroom.h"

static struct scene scene;
static f32 angle_last, angle;

void testroom_load(void)
{
	scene_read_file(&scene, "rom:/test.scn");
	angle_last = angle = 0.0f;
}

void testroom_unload(void)
{
	scene_destroy(&scene);
}

enum scene_index testroom_update(struct input_parms iparms)
{
	if (iparms.press.start)
		return (SCENE_TITLE);

	angle_last = angle;
	angle += 2.4f;

	return (SCENE_TESTROOM);
}

void testroom_draw(f32 subtick)
{
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3);
	glRotatef(lerpf(angle_last, angle, subtick), 0, 0, 1);
	glRotatef(lerpf(angle_last, angle, subtick), 0, 1, 0);

	scene_draw(&scene);
}
