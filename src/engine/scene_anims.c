#include <assert.h>

#include "engine/animation.h"
#include "engine/scene.h"

/**
 * scene_anims_update - Updates all Animations in a Scene Struct
 * @s: Scene to Update Animations for
 */
void scene_anims_update(struct scene *s)
{
	for (u16 i = 0; i < s->num_anims; i++)
		animation_update(s->anims + i);
}

/**
 * scene_anims_set_flags - Sets Flags for All Animations
 * @s: Scene to set Animation Flags For
 * @flags: Flags themselves
 */
void scene_anims_set_flags(struct scene *s, u8 flags)
{
	for (u16 i = 0; i < s->num_anims; i++)
		s->anims[i].flags = flags;
}

/**
 * scene_anims_set_frame - Sets Frame for All Animations
 * @s: Scene to set Animation Frame For
 * @frame: Frame itself
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
