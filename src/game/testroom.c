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
 * testroom_load - Loads assets for Testroom
 */
void testroom_load(void)
{
	depth_buffer = surface_alloc(FMT_RGBA16, CONF_WIDTH, CONF_HEIGHT);

	camera_init(&cam);
	scene_read_file(&scene, "rom:/Test.scn");
	player_init(&scene, &player, ITEM_HAS_NONE);
	crosshair_block = crosshair_rspq_block_gen(7);
	tick_cnt_last = tick_cnt = 0;
}

/**
 * testroom_unload - Unloads assets for Testroom
 */
void testroom_unload(void)
{
	scene_destroy(&scene);
	rspq_block_free(crosshair_block);
	rspq_block_free(weed_high_block);
	surface_free(&depth_buffer);
}

/**
 * testroom_update - Updates Testroom and all its stuff
 * @iparms: Input Parameters
 *
 * Return: Desired Next Scene Index
 */
enum scene_index testroom_update(struct input_parms iparms)
{
	tick_cnt_last = tick_cnt++;

	if (iparms.press.start)
		testroom_flags ^= TR_FREECAM_ENABLED;

	if (testroom_flags & TR_FREECAM_ENABLED)
		camera_update(&cam, iparms);
	else
		player_update(&scene, &player, iparms);

	scene_anims_update(&scene);
	player_items_update(&player, iparms);

	ui_timer_update();

	return (SCENE_TESTROOM);
}

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

	player_item_draw(&player, subtick);

	glDisable(GL_DEPTH_TEST);
	gl_context_end();
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
 * testroom_draw - Draws all the stuff for Testroom
 * @subtick: Delta Value Between Frames
 */
void testroom_draw(f32 subtick)
{
	color_buffer = display_get();
	rdpq_attach(color_buffer, &depth_buffer);
	_testroom_render(subtick);
	if (player.weed_high_amnt > 0)
	{
		const f32 tick_cnt_lerp = lerpf(tick_cnt_last,
				  tick_cnt, subtick);
		const f32 intensity =
			clampf((f32)player.weed_progress /
			((f32)player.weed_duration * 0.25f), 0, 1);
		const f32 scale = lerpf(1.0f, 1.04f, intensity);

		rdpq_set_mode_standard();
		rdpq_set_fog_color(RGBA32(0, 0, 0, intensity * 255));
		rdpq_mode_blender(RDPQ_BLENDER_ADDITIVE);
		rdpq_tex_blit(color_buffer, (CONF_WIDTH >> 1) +
		sinf(tick_cnt_lerp * 0.1f) * intensity * 4,
		       (CONF_HEIGHT >> 1) +
		cosf(tick_cnt_lerp * 0.1f) * intensity * 2,
		       &(const rdpq_blitparms_t) {
			.cx = (CONF_WIDTH >> 1),
			.cy = (CONF_HEIGHT >> 1),
			.scale_x = scale,
			.scale_y = scale,
		});
	}
	rdpq_detach_show();
}
