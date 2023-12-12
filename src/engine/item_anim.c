#include "engine/item.h"

/**
 * item_anim_update - Updates all Animations for Item
 * @item: Item to Update Current Animation
 */
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

/**
 * item_anim_at_end - Whether an Item's Animation is at its End
 * @item: Item to Check Anim End
 * @anim_index: Animation Index to Check
 *
 * Return: Whether that Animation is at its end
 */
u8 item_anim_at_end(const struct item *item, const u16 anim_index)
{
	const struct animation *a = item->s.anims + anim_index;

	return (a->frame == a->length - 1);
}

/**
 * item_anim_is_index - Whether the Item's Current Animation is index
 * @item: Item to Check Index
 * @anim_index: Animation Index Desired
 *
 * Return: Whether the Item's Current Animation is the index specified
 */
u8 item_anim_is_index(const struct item *item, const u16 anim_index)
{
	return (anim_index == item->anim_index);
}

/**
 * item_anim_set_flags - Sets Flags for an Item's Specific Animation
 * @item: Item to Set Anim Flags
 * @anim_index: Desired Animation Index
 * @flags: The Flags in Question
 */
void item_anim_set_flags(struct item *item,
			 const u16 anim_index, const u8 flags)
{
	item->s.anims[anim_index].flags = flags;
}

/**
 * item_anim_set_frame - Sets Frame for Item's Specific Animation
 * @item: Item
 * @anim_index: Animation Index
 * @frame: Desired Frame
 */
void item_anim_set_frame(struct item *item,
			 const u16 anim_index, const u16 frame)
{
	item->s.anims[anim_index].frame = frame;
}
