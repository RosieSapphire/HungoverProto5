#ifndef _ENGINE_ITEM_H_
#define _ENGINE_ITEM_H_

/**
 * @file include/engine/item.h
 */

#include "engine/scene.h"

/**
 * Item Structure
 */
struct item
{
	struct scene s; ///< Scene for Mesh and Stuff
	u16 anim_index; ///< Animation currently playing
	u16 qty1; ///< Quantity of whatever it may be (bullets, drugs, etc.)
	u16 qty2; ///< 2nd Quantity (Only used for Weapons as a denominator)
	u32 usage_timer; ///< Used for stuff that requires holding the button
	u32 usage_timer_last; ///< Usage Timer Last (Used for Lerping)
};

/*
 * Main
 */
void item_node_draw(const struct item *item,
		    const struct node *n, const f32 subtick);

/*
 * Animations
 */
void item_anim_update(struct item *item);
u8 item_anim_at_end(const struct item *item, const u16 anim_index);
u8 item_anim_is_index(const struct item *item, const u16 anim_index);
void item_anim_set_flags(struct item *item, const u16 anim_index,
			 const u8 flags);
void item_anim_set_frame(struct item *item, const u16 anim_index,
			 const u16 frame);

#endif /* _ENGINE_ITEM_H_ */
