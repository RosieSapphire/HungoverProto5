#include <GL/gl.h>
#include <GL/gl_integration.h>
#include <libdragon.h>

#include "engine/util.h"
#include "engine/config.h"
#include "engine/sfx.h"
#include "engine/scene.h"
#include "engine/texture.h"

#include "game/title.h"
#include "game/testroom.h"

static surface_t *color_buffer;
static surface_t depth_buffer;

static void (*load_funcs[NUM_SCENES])(void) = {
	title_load, testroom_load,
};

static void (*unload_funcs[NUM_SCENES])(void) = {
	title_unload, testroom_unload,
};

enum scene_index (*update_funcs[NUM_SCENES])(struct input_parms) = {
	title_update, testroom_update,
};

static void (*draw_funcs[NUM_SCENES])(f32) = {
	title_draw, testroom_draw,
};

/**
 * _init - Initialization Function
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

	debug_init_isviewer();
	debug_init_usblog();

	sfx_load();

	gl_init();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	projection_setup();

	depth_buffer = surface_alloc(FMT_RGBA16, 320, 240);

	(*load_funcs[scene_index])();
}

/**
 * _draw - Drawing Function
 * @subtick: Delta Value Between Frames
 */
static void _draw(f32 subtick)
{
	color_buffer = display_get();
	rdpq_attach(color_buffer, &depth_buffer);
	(*draw_funcs[scene_index])(subtick);
	rdpq_detach_show();
}

/**
 * _audio - Updates Audio
 */
static void _audio(void)
{
	if (!audio_can_write())
		return;

	mixer_poll(audio_write_begin(), audio_get_buffer_length());
	audio_write_end();
}

/**
 * main - Main Function
 *
 * Return: 0 = Success
 */
int main(void)
{
	u32 ticks_last, ticks_now, ticks_accum, ticks_delta;

	_init();
	ticks_last = ticks_accum = 0;
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

		_draw(subtick);
		_audio();
	}
	return (0);
}
