/**
 * @file src/engine/item_anim.c
 */

#include "engine/item.h"

/**
 * Updates all Animations for Item
 * @param[in,out] item Item to Update Current Animation
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
 * Whether an Item's Animation is at its End
 * @param[in] item Item to Check Anim End
 * @param[in] anim_index Animation Index to Check
 * @return Whether that Animation is at its end
 */
u8 item_anim_at_end(const struct item *item, const u16 anim_index)
{
	const struct animation *a = item->s.anims + anim_index;

	return (a->frame == a->length - 1);
}

/**
 * Whether the Item's Current Animation is index
 * @param[in] item Item to Check Index
 * @param[in] anim_index Animation Index Desired
 * @return Whether the Item's Current Animation is the index specified
 */
u8 item_anim_is_index(const struct item *item, const u16 anim_index)
{
	return (anim_index == item->anim_index);
}

/**
 * Sets Flags for an Item's Specific Animation
 * @param[out] item Item to Set Anim Flags
 * @param[in] anim_index Desired Animation Index
 * @param[in] flags The Flags in Question
 */
void item_anim_set_flags(struct item *item, const u16 anim_index,
			 const u8 flags)
{
	item->s.anims[anim_index].flags = flags;
}

/**
 * Sets Frame for Item's Specific Animation
 * @param[out] item Item
 * @param[in] anim_index Animation Index
 * @param[in] frame Desired Frame
 */
void item_anim_set_frame(struct item *item, const u16 anim_index,
			 const u16 frame)
{
	item->s.anims[anim_index].frame = frame;
}
