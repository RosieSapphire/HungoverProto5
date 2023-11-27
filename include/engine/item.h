#ifndef _ENGINE_ITEM_H_
#define _ENGINE_ITEM_H_

#include "engine/scene.h"

struct item
{
	struct scene s;
	u16 anim_index;
};

void item_scene_anim_update(struct item *item);
void item_scene_node_draw(const struct item *item, const struct node *n,
		     const f32 subtick);

#endif /* _ENGINE_ITEM_H_ */
