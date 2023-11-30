#ifndef _ENGINE_ITEM_H_
#define _ENGINE_ITEM_H_

#include "engine/scene.h"

struct item
{
	struct scene s;
	u16 anim_index;
	u16 qty1, qty2;
};

void item_anim_update(struct item *item);
void item_node_draw(const struct item *item,
		    const struct node *n, const f32 subtick);
u8 item_anim_is_playing(const struct item *item, const u16 anim_index);

#endif /* _ENGINE_ITEM_H_ */
