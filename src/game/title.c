#include <GL/gl.h>
#include <GL/gl_integration.h>

#include "engine/util.h"
#include "engine/sfx.h"
#include "engine/texture.h"
#include "engine/scene.h"

#include "game/title.h"

static struct texture title_text;
static struct texture proto_text;
static struct scene scene;
static f32 timer_last, timer;

/**
 * title_load - Initializes Title Screen
 */
void title_load(void)
{
	const struct vertex verts[] = {
		{{-64, -32, 0}, {0.0f, 1.0f}, {0xFF, 0xFF, 0xFF, 0xFF}},
		{{ 64, -32, 0}, {1.0f, 1.0f}, {0xFF, 0xFF, 0xFF, 0xFF}},
		{{-64,  32, 0}, {0.0f, 0.0f}, {0xFF, 0xFF, 0xFF, 0xFF}},
		{{ 64,  32, 0}, {1.0f, 0.0f}, {0xFF, 0xFF, 0xFF, 0xFF}},
	};

	const u16 indis[] = {0, 1, 2, 2, 1, 3};

	title_text = texture_create_file("rom:/title_text.ia8.sprite");
	proto_text = texture_create_file("rom:/proto5.ia8.sprite");

	scene.num_meshes = 1;
	scene.meshes = malloc(sizeof(struct mesh));
	mesh_create_data(scene.meshes, "Quad", 4, 6, verts, indis);
	scene.num_anims = 0;
	scene.anims = NULL;

	timer_last = timer = 0.0f;

	mixer_ch_set_vol(SFXC_MUSIC, 0.5f, 0.5f);
	wav64_play(&title_music, SFXC_MUSIC);
}

/**
 * title_unload - Unloads Title Screen
 */
void title_unload(void)
{
	timer_last = timer = 0.0f;
	scene_destroy(&scene);
	texture_destroy(&title_text);
}

/**
 * title_update - Updates the Title Screen
 * @iparms: Input Parameters
 *
 * Return: New Desired Scene Index
 */
enum scene_index title_update(struct input_parms iparms)
{
	timer_last = timer;
	timer += 0.1f;

	if (iparms.press.start)
	{
		mixer_ch_stop(SFXC_MUSIC);
		return (SCENE_TESTROOM);
	}

	return (SCENE_TITLE);
}

/**
 * _title_setup_wiggle - Sets up Model Matrix with Wiggle for Title
 * @timer_lerp: Timer lerped with Subtick
 * @offset: Offsets the Jiggle effect by time
 */
static void _title_setup_wiggle(f32 timer_lerp, u8 offset)
{
	const f32 pos_off[3] = {
		sinf(timer_lerp + offset) * 3,
		sinf((timer_lerp + offset) * 1.5f) * 3,
		0,
	};
	const f32 rot_off = sinf(timer_lerp + offset) * 4;

	glTranslatef(pos_off[0], pos_off[1], pos_off[2]);
	glRotatef(rot_off, 0, 0, 1);
}

/**
 * title_draw - Draws the Title Screen
 * @subtick: Delta Value between Frames
 */
void title_draw(f32 subtick)
{
	gl_context_begin();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	const f32 timer_lerp = lerpf(timer_last, timer, subtick);
	const f32 stl1 = (sinf(timer_lerp) + 1.0f) / 4.0f;
	const f32 stl2 = (sinf(timer_lerp + 69) + 1.0f) / 4.0f;
	const f32 ctl = (cosf(timer_lerp) + 1.0f) / 4.0f;

	glClearColor(stl1, ctl, stl2, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const f32 intro_zoom_value =
		smoothf(0, 720, fminf(timer_lerp * 0.35f, 1.0f));
	const struct mesh *text_mesh = scene.meshes + 0;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	_title_setup_wiggle(timer_lerp, 0);
	glTranslatef(0, 0, ((intro_zoom_value - 720) * 0.356f) - 128.0f);
	glRotatef(intro_zoom_value, 0, 0, 1);
	mesh_draw(text_mesh);
	const f32 timer_lerp_slow = timer_lerp * 0.333f;
	u8 blink = (u8)((timer_lerp_slow - (u32)timer_lerp_slow) * 3.0f);

	if (!blink)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		return;
	}

	glLoadIdentity();
	glTranslatef(0, -64, -128);
	glScalef(1.0f, 0.5f, 1.0f);
	mesh_draw(text_mesh);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	gl_context_end();
}
