/**
 * @file src/main.c
 */

#include <GL/gl.h>
#include <GL/gl_integration.h>

#include "engine/texture.h"
#include "engine/ui.h"
#include "engine/sfx.h"
#include "engine/decal.h"
#include "engine/util.h"

#include "game/title.h"
#include "game/testroom.h"

/**
 * Loading functions for all scenes
 */
static void (*load_funcs[NUM_SCENES])(void) = {
	title_load, testroom_load,
};

/**
 * Unloading functions for all scenes
 */
static void (*unload_funcs[NUM_SCENES])(void) = {
	title_unload, testroom_unload,
};

/**
 * Update functions for all scenes
 */
static enum scene_index (*update_funcs[NUM_SCENES])(struct input_parms) = {
	title_update, testroom_update,
};

/**
 * Draw functions for all scenes
 */
static void (*draw_funcs[NUM_SCENES])(f32) = {
	title_draw, testroom_draw,
};

/**
 * Initialization function for game
 */
static void _init(void)
{
	srand(TICKS_READ());
	display_init((resolution_t){CONF_WIDTH, CONF_HEIGHT, 0},
		     DEPTH_32_BPP, 3, GAMMA_CORRECT_DITHER, FILTERS_RESAMPLE);
	rdpq_init();
	dfs_init(DFS_DEFAULT_LOCATION);
	joypad_init();
	textures_init();
	ui_font_init();

#ifdef CONF_DEBUG
	debug_init_isviewer();
	debug_init_usblog();
#endif /* CONF_DEBUG */

	sfx_load();

	gl_init();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	projection_setup();
	
	decals_load();

	(*load_funcs[scene_index])();
}

/**
 * Audio updating
 */
static void _audio(void)
{
	if (!audio_can_write())
		return;

	mixer_poll(audio_write_begin(), audio_get_buffer_length());
	audio_write_end();
}

/**
 * Main function
 * @return 0 = Success
 */
int main(void)
{
	u32 ticks_last = get_ticks(), ticks_now, ticks_accum = 0, ticks_delta;

	_init();
	while (1)
	{
		ticks_now = get_ticks();
		ticks_delta = TICKS_DISTANCE(ticks_last, ticks_now);
		ticks_last = ticks_now;
		ticks_accum += ticks_delta;
		while (ticks_accum >= CONF_DELTATICKS)
		{
			joypad_poll();
			struct input_parms iparms = {
				joypad_get_buttons_pressed(JOYPAD_PORT_1),
				joypad_get_buttons_held(JOYPAD_PORT_1),
				joypad_get_buttons_released(JOYPAD_PORT_1),
				joypad_get_inputs(JOYPAD_PORT_1),
			};

			const enum scene_index scene_ind_last = scene_index;

			scene_index = (*update_funcs[scene_index])(iparms);

			if (scene_ind_last != scene_index)
			{
				(*load_funcs[scene_index])();
				(*unload_funcs[scene_ind_last])();
			}

			scene_global_spin_timer_update();

			ticks_accum -= CONF_DELTATICKS;
		}

		const f32 subtick = (f32)ticks_accum / (f32)CONF_DELTATICKS;

		(*draw_funcs[scene_index])(subtick);
		_audio();
	}
	return (0);
}
