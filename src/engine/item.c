/**
 * @file src/engine/item.c
 */

#include <GL/gl.h>

#include "engine/item.h"

/**
 * Recursively draws nodes for an item
 * @param[in] item Item to draw
 * @param[in] n Node to reference
 * @param[in] subtick Subtick between frames
 */
void item_node_draw(const struct item *item,
		    const struct node *n, const f32 subtick)
{
	if (n->mesh_ind == 0xFFFF)
	{
		for (u16 i = 0; i < n->num_children; i++)
			item_node_draw(item, n->children + i, subtick);

		return;
	}
	const struct mesh *m = item->s.meshes + n->mesh_ind;
	u16 anim_ind = 0xFFFF;

	for (u16 i = 0; i < item->s.num_anims; i++)
	{
		const struct animation *anim = item->s.anims + i;

		if (anim->name[0] != ('0' + item->anim_index))
			continue;

		if (strncmp(m->name, anim->name + 2, strlen(m->name)))
			continue;

		anim_ind = i;
		break;
	}
	if (!(m->flags & MESH_IS_ACTIVE))
	{
		for (u16 i = 0; i < n->num_children; i++)
			item_node_draw(item, n->children + i, subtick);
		return;
	}
	glPushMatrix();
	if (anim_ind != 0xFFFF)
		animation_matrix_multiply(item->s.anims + anim_ind, subtick);
	else
		glMultMatrixf((f32 *)n->trans);
	mesh_draw(m);
	for (u16 i = 0; i < n->num_children; i++)
		item_node_draw(item, n->children + i, subtick);
	glPopMatrix();
}
