#include <GL/gl.h>

#include "engine/util.h"
#include "engine/vector.h"
#include "engine/animation.h"
#include "engine/scene.h"

enum scene_index scene_index = SCENE_TITLE;

void scene_anims_update(struct scene *s)
{
	for (u16 i = 0; i < s->num_anims; i++)
		animation_update(s->anims + i);
}

static void _scene_draw_node(const struct scene *s,
			     const struct node *n, const f32 subtick)
{
	if (n->mesh_ind == 0xFFFF)
	{
		for (int i = 0; i < n->num_children; i++)
			_scene_draw_node(s, n->children + i, subtick);

		return;
	}

	const struct mesh *m = s->meshes + n->mesh_ind;
	u16 anim_ind = 0xFFFF;

	for (u16 i = 0; i < s->num_anims; i++)
	{
		if (s->anims[i].mesh_ind == n->mesh_ind)
		{
			anim_ind = i;
			break;
		}
	}

	glPushMatrix();

	if (anim_ind != 0xFFFF)
		animation_matrix_multiply(s->anims + anim_ind, subtick);
	else
		glMultMatrixf((f32 *)n->trans);

	mesh_draw(m, 0);

	for (u16 i = 0; i < n->num_children; i++)
		_scene_draw_node(s, n->children + i, subtick);

	glPopMatrix();
}

/**
 * scene_draw - Draws a Scene and its Meshes
 * @s: Scene to Draw
 * @subtick: Subtick
 */
void scene_draw(const struct scene *s, const f32 subtick)
{
	_scene_draw_node(s, &s->root_node, subtick);
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

/**
 * scene_destroy - Destroys a Scene
 * @s: Scene to Destroy
 */
void scene_destroy(struct scene *s)
{
	for (u16 i = 0; i < s->num_meshes; i++)
		mesh_destroy(s->meshes + i);

	s->num_meshes = 0;
	s->num_anims = 0;
}
