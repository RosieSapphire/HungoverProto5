#include <libdragon.h>

#include "engine/config.h"
#include "engine/ui.h"

static rdpq_font_t *ui_font;
static u32 frame_counter;

void ui_font_init(void)
{
	ui_font = rdpq_font_load("rom:/font.font64");
	rdpq_font_style(ui_font, 0, &(rdpq_fontstyle_t){
		.color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF),
	});
	rdpq_text_register_font(1, ui_font);
	frame_counter = 0;
}

void ui_timer_update(void)
{
	frame_counter++;
}

void ui_prototype_draw(void)
{
	if ((frame_counter >> 4) & 1)
		rdpq_text_printf(NULL, 1, 32, 32, "PROTOTYPE BUILD 5");
}

void ui_item_qty_draw(const struct item *i, const u8 uses_2nd)
{
	const char *fmt = uses_2nd ? "Ammo: %d/%d" : "Qty: %d";

	rdpq_text_printf(NULL, 1, 32, CONF_HEIGHT - 32,
		  fmt, i->qty1, uses_2nd ? i->qty2 : 0);
}
