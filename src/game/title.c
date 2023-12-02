#include <GL/gl.h>
#include <GL/gl_integration.h>

#include "engine/util.h"
#include "engine/sfx.h"
#include "engine/texture.h"
#include "engine/scene.h"

#include "game/title.h"

static surface_t *color_buffer, depth_buffer;

static struct scene scene;
static f32 beat_counter_last, beat_counter;
static u8 title_music_state;

/**
 * title_load - Initializes Title Screen
 */
void title_load(void)
{
	f32 start = (f32)get_ticks() / (f32)TICKS_PER_SECOND;

	scene_read_file(&scene, "rom:/Title.scn");
	scene_anims_set_flags(&scene, ANIM_IS_PLAYING);
	scene_anims_set_frame(&scene, 0);
	depth_buffer = surface_alloc(FMT_RGBA16, CONF_WIDTH, CONF_HEIGHT);

	title_music_state = TM_INTRO;
	mixer_ch_set_vol(SFXC_MUSIC0, 0.5f, 0.5f);
	wav64_play(&title_music_intro, SFXC_MUSIC0);
	mixer_ch_set_vol(SFXC_MUSIC1, 0.0f, 0.0f);
	wav64_play(&title_music_init, SFXC_MUSIC1);
	mixer_ch_set_vol(SFXC_MUSIC2, 0.0f, 0.0f);
	wav64_play(&title_music_main, SFXC_MUSIC2);
	f32 end = (f32)get_ticks() / (f32)TICKS_PER_SECOND;

	beat_counter_last = beat_counter = -(end - start);
}

/**
 * title_unload - Unloads Title Screen
 */
void title_unload(void)
{
	beat_counter_last = beat_counter = 0.0f;
	scene_destroy(&scene);
	surface_free(&depth_buffer);
}

/**
 * title_update - Updates the Title Screen
 * @iparms: Input Parameters
 *
 * Return: New Desired Scene Index
 */
enum scene_index title_update(struct input_parms iparms)
{
	beat_counter_last = beat_counter;
	beat_counter += 0.1215375f;

	scene_anims_update(&scene);

	if (title_music_state == TM_INTRO && (u16)beat_counter >= 32)
	{
		title_music_state = TM_INIT;
		mixer_ch_set_vol(SFXC_MUSIC0, 0.0f, 0.0f);
		mixer_ch_set_vol(SFXC_MUSIC1, 0.5f, 0.5f);
		return (SCENE_TITLE);
	}

	if (title_music_state == TM_INIT && iparms.press.start)
	{
		title_music_state = TM_MAIN;
		mixer_ch_set_vol(SFXC_MUSIC1, 0.0f, 0.0f);
		mixer_ch_set_vol(SFXC_MUSIC2, 0.5f, 0.5f);
		return (SCENE_TITLE);
	}

	if (title_music_state == TM_MAIN && iparms.press.start)
	{
		mixer_ch_stop(SFXC_MUSIC0);
		mixer_ch_stop(SFXC_MUSIC1);
		mixer_ch_stop(SFXC_MUSIC2);
		return (SCENE_TESTROOM);
	}

	return (SCENE_TITLE);
}

/**
 * _title_setup_wiggle - Sets up Model Matrix with Wiggle for Title
 * @beat_counter_lerp: Timer lerped with Subtick
 * @offset: Offsets the Jiggle effect by time
 */
static void _title_setup_wiggle(f32 beat_counter_lerp, u8 offset)
{
	beat_counter_lerp *= 1;

	const f32 pos_off[3] = {
		sinf(beat_counter_lerp + offset) * 0.1f,
		sinf((beat_counter_lerp + offset) * 1.5f) * 0.1f,
		0,
	};
	const f32 rot_off = sinf(beat_counter_lerp + offset) * 4;

	glTranslatef(pos_off[0], pos_off[1], pos_off[2]);
	glRotatef(rot_off, 0, 0, 1);
}

/**
 * title_draw - Draws the Title Screen
 * @subtick: Delta Value between Frames
 */
void title_draw(f32 subtick)
{
	color_buffer = display_get();
	rdpq_attach(color_buffer, &depth_buffer);
	gl_context_begin();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);
	const f32 beat_counter_lerp = lerpf(beat_counter_last,
				     beat_counter, subtick);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	_title_setup_wiggle(beat_counter_lerp, 0);
	scene_draw(&scene, subtick);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	gl_context_end();
	rdpq_detach_show();
}
