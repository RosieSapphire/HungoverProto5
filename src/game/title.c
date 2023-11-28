#include <GL/gl.h>
#include <GL/gl_integration.h>

#include "engine/util.h"
#include "engine/sfx.h"
#include "engine/texture.h"
#include "engine/scene.h"

#include "game/title.h"

enum title_music_state : u8
{
	TM_INTRO = 0,
	TM_INIT,
	TM_MAIN,
	TM_COUNT,
};

static struct scene scene;
static f32 timer_last, timer;
static u8 title_music_state;

/**
 * title_load - Initializes Title Screen
 */
void title_load(void)
{
	scene_read_file(&scene, "rom:/Title.scn");
	scene_anims_set_flags(&scene, ANIM_IS_PLAYING);
	scene_anims_set_frame(&scene, 0);

	timer_last = timer = 0.0f;

	title_music_state = TM_INTRO;
	mixer_ch_set_vol(SFXC_MUSIC, 0.5f, 0.5f);
	wav64_play(&title_music_intro, SFXC_MUSIC);
}

/**
 * title_unload - Unloads Title Screen
 */
void title_unload(void)
{
	timer_last = timer = 0.0f;
	scene_destroy(&scene);
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
	timer += 0.0057142857f;

	scene_anims_update(&scene);

	if (title_music_state == TM_INTRO && timer >= 1.53f)
	{
		title_music_state = TM_INIT;
		wav64_play(&title_music_init, SFXC_MUSIC);
		return (SCENE_TITLE);
	}

	if (title_music_state == TM_INIT && iparms.press.start)
	{
		title_music_state = TM_MAIN;
		wav64_play(&title_music_main, SFXC_MUSIC);
		return (SCENE_TITLE);
	}

	if (title_music_state == TM_MAIN && iparms.press.start)
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
	timer_lerp *= 17.5f;

	const f32 pos_off[3] = {
		sinf(timer_lerp + offset) * 0.1f,
		sinf((timer_lerp + offset) * 1.5f) * 0.1f,
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
	glDisable(GL_CULL_FACE);
	const f32 timer_lerp = lerpf(timer_last, timer, subtick);
	/*
	const f32 stl1 = (sinf(timer_lerp) + 1.0f) / 4.0f;
	const f32 stl2 = (sinf(timer_lerp + 69) + 1.0f) / 4.0f;
	const f32 ctl = (cosf(timer_lerp) + 1.0f) / 4.0f;
	*/

	// glClearColor(stl1, ctl, stl2, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	const f32 intro_zoom_value =
		smoothf(0, 720, fminf(timer_lerp * 0.35f, 1.0f));
	const struct mesh *text_mesh = scene.meshes + 0;
	*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	_title_setup_wiggle(timer_lerp, 0);
	// glTranslatef(0, 0, ((intro_zoom_value - 720) * 0.356f) - 128.0f);
	// glRotatef(intro_zoom_value, 0, 0, 1);
	scene_draw(&scene, subtick);
	// const f32 timer_lerp_slow = timer_lerp * 0.333f;
	// u8 blink = (u8)((timer_lerp_slow - (u32)timer_lerp_slow) * 3.0f);

	/*
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

	*/
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	gl_context_end();
}
