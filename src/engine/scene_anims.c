/**
 * @file src/engine/scene_anims.c
 */

#include <assert.h>

#include "engine/animation.h"
#include "engine/scene.h"

/**
 * Updates all Animations in a Scene Struct
 * @param[in,out] s Scene to Update Animations for
 */
void scene_anims_update(struct scene *s)
{
	for (u16 i = 0; i < s->num_anims; i++)
		animation_update(s->anims + i);
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
