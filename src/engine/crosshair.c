#include "engine/config.h"
#include "engine/crosshair.h"

rspq_block_t *crosshair_rspq_block_gen(const u16 dim)
{
	rspq_block_begin();
	rdpq_set_mode_fill(RGBA32(0xFF, 0xFF, 0xFF, 0xFF));
	const u16 cx = (CONF_WIDTH >> 1) - (dim >> 1);
	const u16 cy = (CONF_HEIGHT >> 1) - (dim >> 1);

	rdpq_fill_rectangle(cx + (dim >> 1), cy, cx + (dim >> 1) + 1, cy + dim);
	rdpq_fill_rectangle(cx, cy + (dim >> 1), cx + dim, cy + (dim >> 1) + 1);
	return (rspq_block_end());
}

void crosshair_draw(rspq_block_t *crosshair_block)
{
	rspq_block_run(crosshair_block);
}
