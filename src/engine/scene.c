#include <GL/gl.h>

#include "engine/scene.h"

enum scene_index scene_index = SCENE_TITLE;

static const struct node *_scene_node_from_mesh_ind(const struct node *n,
					      const u16 mesh_ind)
{
	if (n->mesh_ind == mesh_ind)
		return (n);

	const struct node *nf;

	for (u16 i = 0; i < n->num_children; i++)
		if ((nf = _scene_node_from_mesh_ind(n->children + i, mesh_ind)))
			return (nf);

	return (NULL);
}

/**
 * scene_draw - Draws a Scene and its Meshes
 * @s: Scene to Draw
 */
void scene_draw(const struct scene *s)
{
	for (u16 i = 0; i < s->num_meshes; i++)
	{
		const struct mesh *m = s->meshes + i;
		const struct node *n =
			_scene_node_from_mesh_ind(&s->root_node, i);

		if (!n)
			continue;

		glPushMatrix();
		glMultMatrixf((f32 *)n->trans);
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
