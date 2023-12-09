#include <GL/gl.h>

#include "engine/util.h"
#include "engine/vector.h"
#include "engine/scene.h"

enum scene_index scene_index = SCENE_TESTROOM;
static f32 spin_timer_last, spin_timer;

void scene_global_spin_timer_update(void)
{
	spin_timer_last = spin_timer;
	spin_timer += 1.2f;
}

static void _scene_draw_node_pickup(const struct scene *s,
				    const struct node *n, const struct mesh *m,
				    const f32 subtick)
{
	const f32 spin_timer_lerp = lerpf(spin_timer_last, spin_timer, subtick);

	glMultMatrixf((f32 *)n->trans);
	glTranslatef(0, sinf(spin_timer_lerp * 0.2f) * 0.2f, 0);
	glRotatef(spin_timer_lerp * 8, 0, 1, 0);
	mesh_draw(m);

	for (u16 i = 0; i < n->num_children; i++)
		_scene_draw_node_pickup(s, n->children + i, m, subtick);

	glPopMatrix();
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
		if (s->anims[i].mesh_ind != n->mesh_ind)
			continue;

		anim_ind = i;
		break;
	}
	if (!(m->flags & MESH_IS_ACTIVE))
	{
		for (u16 i = 0; i < n->num_children; i++)
			_scene_draw_node(s, n->children + i, subtick);
		return;
	}
	glPushMatrix();
	if (!strncmp("PU.", m->name, 3))
	{
		_scene_draw_node_pickup(s, n, m, subtick);
		return;
	}
	if (anim_ind != 0xFFFF)
		animation_matrix_multiply(s->anims + anim_ind, subtick);
	else
		glMultMatrixf((f32 *)n->trans);

	mesh_draw(m);
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
