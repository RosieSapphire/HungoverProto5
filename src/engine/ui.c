/**
 * @file src/engine/ui.c
 */

#include <libdragon.h>

#include "engine/util.h"
#include "engine/config.h"
#include "engine/ui.h"

static rdpq_font_t *ui_font;
static sprite_t *bongometer_spr;
static sprite_t *bongometer_arrow_spr;
static u32 frame_counter;

/**
 * Initializes the UI Font
 */
void ui_font_init(void)
{
	ui_font = rdpq_font_load("rom:/font.font64");
	rdpq_font_style(ui_font, 0, &(rdpq_fontstyle_t){
		.color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF),
	});
	rdpq_text_register_font(1, ui_font);
	bongometer_spr = sprite_load("rom:/bongometer.ci8.sprite");
	bongometer_arrow_spr = sprite_load("rom:/bongometer_arrow.ia4.sprite");
	frame_counter = 0;
}

/**
 * Updates the UI Timer
 */
void ui_timer_update(void)
{
	frame_counter++;
}

/**
 * Draws the PROTOTYPE BUILD 5 Text in the topleft corner
 */
void ui_prototype_draw(void)
{
	if ((frame_counter >> 4) & 1)
		rdpq_text_printf(NULL, 1, 32, 32, "PROTOTYPE BUILD 5");
}

/**
 * Draws the Quantity of an Item
 * @param[in] i Item whose quantity is to be displayed
 * @param[in] uses_2nd Whether or not it uses the 2nd Quantity available
 */
void ui_item_qty_draw(const struct item *i, const u8 uses_2nd)
{
	const char *fmt = uses_2nd ? "Ammo: %d/%d" : "Qty: %d";

	rdpq_text_printf(NULL, 1, 32, CONF_HEIGHT - 32,
			 fmt, i->qty1, uses_2nd ? i->qty2 : 0);
}

/**
 * Draws the Bong-O-Meter for Smoking Weed
 * @param[in] item Pretty much only the Bong (p->items + 1)
 * @param[in] subtick Subtick between Frames
 */
void ui_bongometer_draw(const struct item *item, const f32 subtick)
{
	const f32 usage_timer_lerp = clampf(lerpf(item->usage_timer_last,
					    item->usage_timer, subtick), 0, 56);
	const f32 lerpt = clampf(usage_timer_lerp, 0, 8) / 8.0f;
	const f32 y_pos = smoothf((CONF_HEIGHT >> 1) - 256,
		   (CONF_HEIGHT >> 1) - 96, lerpt);

	rdpq_set_mode_standard();
	rdpq_mode_alphacompare(1);
	rdpq_sprite_blit(bongometer_spr, (CONF_WIDTH >> 1) - 64, y_pos, NULL);
	const f32 theta = ((-usage_timer_lerp * usage_timer_lerp *
		usage_timer_lerp * usage_timer_lerp) * 0.0000001875f) + 1.04f;

	rdpq_sprite_blit(bongometer_arrow_spr,
			 (CONF_WIDTH >> 1), y_pos + 100,
			 &(const rdpq_blitparms_t) {
				.scale_x = 0.5f, .scale_y = 0.5f,
				.cx = 8, .cy = 128, .theta = theta,
			}
	);
}
