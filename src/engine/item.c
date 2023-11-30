#include <GL/gl.h>

#include "engine/profiler.h"
#include "engine/item.h"

void item_anim_update(struct item *item)
{
	for (u16 i = 0; i < item->s.num_anims; i++)
	{
		struct animation *a = item->s.anims + i;

		if (a->name[0] != '0' + item->anim_index)
			continue;

		animation_update(a);
	}
}

u8 item_anim_is_playing(const struct item *item, const u16 anim_index)
{
	const struct animation *a = item->s.anims + anim_index;

	return (a->flags & ANIM_IS_PLAYING);
}

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
		if (item->s.anims[i].name[0] != ('0' + item->anim_index))
			continue;

		if (strncmp(m->name,
	      			item->s.anims[i].name + 2, strlen(m->name)))
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
