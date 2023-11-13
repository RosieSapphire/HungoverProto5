#include <GL/gl.h>
#include <GL/gl_integration.h>
#include <libdragon.h>

#include "engine/util.h"
#include "engine/config.h"

#include "game/title.h"

static surface_t *color_buffer;
static surface_t depth_buffer;

/**
 * _init - Initialization Function
 */
static void _init(void)
{
	display_init((resolution_t){CONF_WIDTH, CONF_HEIGHT, 0},
	      DEPTH_32_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
	rdpq_init();
	dfs_init(DFS_DEFAULT_LOCATION);
	debug_init_isviewer();
	debug_init_usblog();

	gl_init();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	projection_setup();

	depth_buffer = surface_alloc(FMT_RGBA16, 320, 240);

	title_load();
}

/**
 * _draw - Drawing Function
 * @subtick: Delta Value Between Frames
 */
static void _draw(f32 subtick)
{
	color_buffer = display_get();
	rdpq_attach(color_buffer, &depth_buffer);
	gl_context_begin();
	title_draw(subtick);
	gl_context_end();
	rdpq_detach_show();
}

/**
 * main - Main Function
 *
 * Return: 0 = Success
 */
int main(void)
{
	_init();

	u32 ticks_last, ticks_now, ticks_accum, ticks_delta;

	ticks_last = ticks_accum = 0;
	while (1)
	{
		ticks_now = get_ticks();
		ticks_delta = TICKS_DISTANCE(ticks_last, ticks_now);
		ticks_last = ticks_now;

		ticks_accum += ticks_delta;
		while (ticks_accum >= CONF_DELTATICKS)
		{
			title_update();
			ticks_accum -= CONF_DELTATICKS;
		}

		const f32 subtick = (f32)ticks_accum / (f32)CONF_DELTATICKS;

		_draw(subtick);
	}

	return (0);
}
