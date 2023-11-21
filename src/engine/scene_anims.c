#include <assert.h>

#include "engine/animation.h"
#include "engine/scene.h"

void scene_anims_update(struct scene *s)
{
	for (u16 i = 0; i < s->num_anims; i++)
		animation_update(s->anims + i);
}

void scene_anims_set_flags(struct scene *s, u8 flags)
{
	for (u16 i = 0; i < s->num_anims; i++)
		s->anims[i].flags = flags;
}

void scene_anims_set_frame(struct scene *s, u16 frame)
{
	for (u16 i = 0; i < s->num_anims; i++)
	{
		struct animation *a = s->anims + i;

		assert(frame < a->length);
		a->frame = frame;
	}
}
