#ifndef _ENGINE_ITEM_H_
#define _ENGINE_ITEM_H_

#include "engine/scene.h"

/**
 * struct item - Item Structure
 * @s: Scene for Mesh and Stuff
 * @anim_index: Animation currently playing
 * @qty1: Quantity of whatever it may be (bullets, drugs, etc.)
 * @qty2: 2nd Quantity (Only used for Weapons as a denominator)
 * @usage_timer: Mainly used for stuff that requires holding the button
 */
struct item
{
	struct scene s;
	u16 anim_index, qty1, qty2;
	u32 usage_timer;
};

void item_anim_update(struct item *item);
void item_node_draw(const struct item *item,
		    const struct node *n, const f32 subtick);
u8 item_anim_is_playing(const struct item *item, const u16 anim_index);
u8 item_anim_at_end(const struct item *item, const u16 anim_index);
u8 item_anim_is_index(const struct item *item, const u16 anim_index);

void item_anim_set_flags(struct item *item,
			 const u16 anim_index, const u8 flags);
void item_anim_set_frame(struct item *item,
			 const u16 anim_index, const u16 frame);

#endif /* _ENGINE_ITEM_H_ */
