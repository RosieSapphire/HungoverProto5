#include <GL/gl.h>

#include "engine/vector.h"
#include "engine/animation.h"
#include "engine/scene.h"

enum scene_index scene_index = SCENE_TESTROOM;

/**
 * _scene_node_from_mesh_ind - Finds a Node from a Mesh Index
 * @n: Node to start searching
 * @mesh_ind: Mesh Index to find
 *
 * Return: Const Pointer to Node Found
 */
static const struct node *_scene_node_from_mesh_ind(const struct node *n,
						    const u16 mesh_ind)
{
	if (n->mesh_ind == mesh_ind)
		return (n);

	const struct node *nf;

	for (u16 i = 0; i < n->num_children; i++)
	{
		nf = _scene_node_from_mesh_ind(n->children + i, mesh_ind);
		if (nf)
			return (nf);
	}

	return (NULL);
}

void scene_anims_update(struct scene *s)
{
	for (u16 i = 0; i < s->num_anims; i++)
		animation_update(s->anims + i);
}

/**
 * scene_draw - Draws a Scene and its Meshes
 * @s: Scene to Draw
 * @subtick: Subtick
 */
void scene_draw(const struct scene *s, const f32 subtick)
{
	static f32 test = 0;

	for (u16 i = 0; i < s->num_meshes; i++)
	{
		const struct mesh *m = s->meshes + i;
		const struct node *n =
			_scene_node_from_mesh_ind(&s->root_node, i);

		test += 0.24f;

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
