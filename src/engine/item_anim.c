#include "engine/item.h"

void item_anim_update(struct item *item)
{
	for (u16 i = 0; i < item->s.num_anims; i++)
	{
		struct animation *a = item->s.anims + i;

		if (a->name[0] != '0' + item->anim_index)
			continue;

		animation_update(a);
	}
}

u8 item_anim_at_end(const struct item *item, const u16 anim_index)
{
	const struct animation *a = item->s.anims + anim_index;

	return (a->frame == a->length - 1);
}

u8 item_anim_is_index(const struct item *item, const u16 anim_index)
{
	return (anim_index == item->anim_index);
}

void item_anim_set_flags(struct item *item,
			 const u16 anim_index, const u8 flags)
{
	item->s.anims[anim_index].flags = flags;
}

void item_anim_set_frame(struct item *item,
			 const u16 anim_index, const u16 frame)
{
	item->s.anims[anim_index].frame = frame;
}
