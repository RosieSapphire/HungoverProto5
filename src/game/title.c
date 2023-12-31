/**
 * @file src/game/title.c
 */

#include <GL/gl.h>
#include <GL/gl_integration.h>

#include "engine/util.h"
#include "engine/sfx.h"
#include "engine/texture.h"
#include "engine/scene.h"

#include "game/title.h"

static surface_t *color_buffer, depth_buffer;

static struct scene scene;
static f32 beat_counter_last;
static f32 beat_counter;
static f32 beat_counter_at_state_change;
static u8 title_music_state_last;
static u8 title_music_state;

/**
 * Initializes Title Screen
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

	beat_counter_last = -(end - start);
	beat_counter = beat_counter_last;
}

/**
 * Unloads Title Screen
 */
void title_unload(void)
{
	beat_counter_last = 0.0f;
	beat_counter = 0.0f;
	scene_destroy(&scene);
	surface_free(&depth_buffer);
}

static enum scene_index _title_update_state(const f32 vol,
					    const struct input_parms iparms)
{
	title_music_state_last = title_music_state;
	switch (title_music_state)
	{
	case TM_INTRO:
		if ((u16)beat_counter_last >= 30)
		{
			title_music_state = TM_INIT;
			return (SCENE_TITLE);
		}
		break;
	case TM_INIT:
		if (iparms.press.start)
		{
			title_music_state = TM_MAIN;
			return (SCENE_TITLE);
		}

		mixer_ch_set_vol(SFXC_MUSIC0, 0.8f - vol, 0.8f - vol);
		mixer_ch_set_vol(SFXC_MUSIC1, vol, vol);
		break;
	case TM_MAIN:
		if (iparms.press.start)
		{
			title_music_state = TM_END;
			mixer_ch_set_vol(SFXC_MUSIC0, 0.8f, 0.8f);
			mixer_ch_set_vol(SFXC_MUSIC1, 0.0f, 0.0f);
			mixer_ch_set_vol(SFXC_MUSIC2, 0.0f, 0.0f);
			wav64_play(&title_music_start, SFXC_MUSIC0);
			return (SCENE_TESTROOM);
		}

		mixer_ch_set_vol(SFXC_MUSIC1, 0.8f - vol, 0.8f - vol);
		mixer_ch_set_vol(SFXC_MUSIC2, vol, vol);
		break;
	default:
		return (SCENE_UNKNOWN);
	}
	return (SCENE_TITLE);
}

/**
 * Updates the Title Screen
 * @param[in] iparms Input Parameters
 * @return New Desired Scene Index
 */
enum scene_index title_update(struct input_parms iparms)
{
	beat_counter_last = beat_counter;
	beat_counter += 0.1215375f;
	scene_anims_update(&scene, (title_music_state == TM_MAIN));
	if (title_music_state_last != title_music_state)
		beat_counter_at_state_change = beat_counter;

	f32 title_music_fade_lerp =
		clampf((beat_counter - beat_counter_at_state_change) * 0.5f,
		       0, 1) * 0.8f;

	return (_title_update_state(title_music_fade_lerp, iparms));
}

/**
 * Draws the Title Screen
 * @param[in] subtick Delta Value between Frames
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

	const f32 pos_off[3] = {
		sinf(beat_counter_lerp) * 0.1f,
		sinf((beat_counter_lerp) * 1.5f) * 0.1f,
		0,
	};
	const f32 rot_off = sinf(beat_counter_lerp) * 4;

	glTranslatef(pos_off[0], pos_off[1], pos_off[2]);
	glRotatef(rot_off, 0, 0, 1);

	scene_draw(&scene, subtick, (title_music_state == TM_MAIN));

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	gl_context_end();
	rdpq_detach_show();
}
