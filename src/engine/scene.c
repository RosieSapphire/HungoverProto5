#include <GL/gl.h>

#include "engine/util.h"
#include "engine/vector.h"
#include "engine/animation.h"
#include "engine/scene.h"

enum scene_index scene_index = SCENE_TESTROOM;

void scene_anims_update(struct scene *s)
{
	for (u16 i = 0; i < s->num_anims; i++)
		animation_update(s->anims + i);
}

static void _scene_draw_node(const struct scene *s,
			     const struct node *n, const f32 subtick,
			     f32 root_mat[4][4])
{
	matrix_mult(root_mat, n->trans, root_mat);

	if (n->mesh_ind == 0xFFFF)
		goto try_children;

	const struct mesh *m = s->meshes + n->mesh_ind;

	glPushMatrix();
	// glMultMatrixf((f32 *)root_mat);
	glMultMatrixf((f32 *)n->trans);
	for (u16 i = 0; i < s->num_anims; i++)
	{
		if (s->anims[i].mesh_ind != n->mesh_ind)
			continue;

		animation_matrix_multiply(s->anims + i, subtick);
	}
	mesh_draw(m, 0);
	glPopMatrix();

try_children:
	for (u16 i = 0; i < n->num_children; i++)
		_scene_draw_node(s, n->children + i, subtick, root_mat);
}

/**
 * scene_draw - Draws a Scene and its Meshes
 * @s: Scene to Draw
 * @subtick: Subtick
 */
void scene_draw(const struct scene *s, const f32 subtick)
{
	f32 ident[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1},
	};

	_scene_draw_node(s, &s->root_node, subtick, ident);

	/*
	for (u16 i = 0; i < s->num_meshes; i++)
	{
		const struct mesh *m = s->meshes + i;
		const struct node *n =
			_scene_node_from_mesh_ind(&s->root_node, i);


		if (!n)
			continue;

		glPushMatrix();
		glMultMatrixf((f32 *)n->trans);
		for (u16 j = 0; j < s->num_anims; j++)
		{
			if (s->anims[j].mesh_ind != i)
				continue;

			animation_matrix_multiply(s->anims + j, subtick);
		}

		mesh_draw(m, 0);
		glPopMatrix();
	}
	*/
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
}
