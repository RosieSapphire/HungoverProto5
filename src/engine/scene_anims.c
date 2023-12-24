/**
 * @file src/engine/scene_anims.c
 */

#include <assert.h>

#include "engine/animation.h"
#include "engine/scene.h"

/**
 * Updates all Animations in a Scene Struct
 * @param[in,out] s Scene to Update Animations for
 * @param[in] index Index to Update
 */
void scene_anims_update(struct scene *s, const u16 index)
{
	for (u16 i = 0; i < s->num_anims; i++)
	{
		struct animation *a = s->anims + i;

		if ((a->name[0] - '0') == index)
		{
			debugf("Update anim '%s'\n", a->name);
			animation_update(a);
		}
	}
}

/**
 * Sets Flags for All Animations
 * @param[out] s Scene to set Animation Flags For
 * @param[in] flags Flags themselves
 */
void scene_anims_set_flags(struct scene *s, u8 flags)
{
	for (u16 i = 0; i < s->num_anims; i++)
		s->anims[i].flags = flags;
}

/**
 * Sets Frame for All Animations
 * @param[in,out] s Scene to set Animation Frame For
 * @param[in] frame Frame itself
 */
void scene_anims_set_frame(struct scene *s, u16 frame)
{
	for (u16 i = 0; i < s->num_anims; i++)
	{
		struct animation *a = s->anims + i;

		assert(frame < a->length);
		a->frame = frame;
	}
}
