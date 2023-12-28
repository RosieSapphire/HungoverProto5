/**
 * @file src/game/testroom.c
 */

#include <GL/gl.h>
#include <GL/gl_integration.h>

#include "engine/vector.h"
#include "engine/util.h"
#include "engine/camera.h"
#include "engine/crosshair.h"
#include "engine/ui.h"
#include "engine/player.h"

#include "game/testroom.h"

static surface_t *color_buffer, depth_buffer;

static struct scene scene;
static struct camera cam;
static struct player player;
static rspq_block_t *crosshair_block, *weed_high_block;

static enum testroom_flags testroom_flags;
static u32 tick_cnt, tick_cnt_last;

/**
 * Loads assets for Testroom
 */
void testroom_load(void)
{
	depth_buffer = surface_alloc(FMT_RGBA16, CONF_WIDTH, CONF_HEIGHT);

	camera_init(&cam);
	scene_read_file(&scene, "rom:/Test.scn");
	player_init(&scene, &player, ITEM_HAS_NONE);
	crosshair_block = crosshair_rspq_block_gen(7);
	tick_cnt_last = 0;
	tick_cnt = 0;
}

/**
 * Unloads assets from Testroom
 */
void testroom_unload(void)
{
	scene_destroy(&scene);
	rspq_block_free(crosshair_block);
	rspq_block_free(weed_high_block);
	surface_free(&depth_buffer);
}

/**
 * Updates Testroom and all its stuff
 * @param[in] iparms Input Parameters
 * @return Desired Next Scene Index
 */
enum scene_index testroom_update(struct input_parms iparms)
{
	tick_cnt_last = tick_cnt++;

	if (iparms.press.start)
		testroom_flags ^= TR_NOCLIP_ENABLED;

	player_update(&scene, &player, iparms,
		      (testroom_flags & TR_NOCLIP_ENABLED));

	scene_anims_update(&scene, 0);
	player_items_update(&player, iparms);

	ui_timer_update();

	return (SCENE_TESTROOM);
}

/**
 * Renders everything for Testroom
 * @param[in] subtick: Subtick between Frames
 */
static void _testroom_render(const f32 subtick)
{
	gl_context_begin();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	player_camera_view_matrix_setup(&player, subtick);
	scene_draw(&scene, subtick, 0);
	player_pistol_decal_draw(&player);
	player_item_draw(&player, subtick);
	gl_context_end();

	/*
	 * Drawing UI
	 */
	const u8 is_weapon = player.item_selected == ITEM_SELECT_PISTOL;
	const struct item *item = player.items + player.item_selected;

	if (is_weapon)
		crosshair_draw(crosshair_block);

	if (player.item_selected != ITEM_SELECT_NONE)
		ui_item_qty_draw(item, is_weapon);

	if (player.item_selected == ITEM_SELECT_BONG &&
	    player.items[1].usage_timer > 0)
		ui_bongometer_draw(item, subtick);

	ui_prototype_draw();
}

/**
 * Draws all the stuff for Testroom
 * @param[in] subtick: Delta Value Between Frames
 */
void testroom_draw(f32 subtick)
{
	color_buffer = display_get();
	rdpq_attach(color_buffer, &depth_buffer);
	_testroom_render(subtick);
	if (player_drug_get_intensity(&player) != 0.0f)
	{
		switch (player.which_drug)
		{
		case ON_DRUG_WEED:
			player_bong_weed_effect_draw(&player, color_buffer,
						     tick_cnt, tick_cnt_last,
						     subtick);
			break;

		case ON_DRUG_NITROUS:
			player_n2o_effect_draw(&player, color_buffer,
					       tick_cnt, tick_cnt_last,
					       subtick);
			break;

		default:
			break;
		}
	}
	rdpq_detach_show();
}
