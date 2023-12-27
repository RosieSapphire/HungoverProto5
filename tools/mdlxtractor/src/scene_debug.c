#include <stdio.h>

#include "scene.h"

void scene_debug_node(const struct node *n, const u8 depth)
{
	for (u8 i = 0; i < depth; i++)
		printf("\t");

	printf("[NODE '%s'] mesh_path='%s', mesh_ind=%d\n",
	       n->name, n->mesh_path, n->mesh_ind);

	for (u16 i = 0; i < n->num_children; i++)
		scene_debug_node(n->children + i, depth + 1);
}

void scene_debug_texture(const char *tpath, const u16 ind)
{
	printf("[TEXTURE %d] '%s'\n", ind, tpath);
}

/**
 * scene_debug_assimp - Debugs everything for Scene Structure
 * @s: Scene to Debug
 */
void scene_debug_assimp(const struct scene *s)
{
	printf("[SCENE] num_meshes=%d\n", s->num_meshes);
	for (u16 i = 0; i < s->num_meshes; i++)
	{
		const struct mesh *m = s->meshes + i;

		printf("\t[MESH %d] name='%s', tind=%u, num_verts=%d\n",
		       i, m->name, m->tind, m->num_verts);
		/*
		for (u16 j = 0; j < m->num_verts; j++)
		{
			const struct vertex *v = m->verts + j;
			const f32 *pos = v->pos;
			const f32 *uv = v->uv;
			const u8 *col = v->col;

			printf("\t\t[POS %d] %.3f, %.3f, %.3f\n",
			       j, pos[0], pos[1], pos[2]);
			printf("\t\t[UV  %d] %.3f, %.3f\n", j, uv[0], uv[1]);
			printf("\t\t[COL %d] %u, %u, %u, %u\n\n",
			       j, col[0], col[1], col[2], col[3]);
		}
      		*/
	}

	scene_debug_node(&s->root_node, 0);

	printf("\n");

	for (u16 i = 0; i < s->num_anims; i++)
	{
		const struct animation *a = s->anims + i;

		printf("[ANIM %d] name='%s'\n", i, a->name);
	}

	for (u16 i = 0; i < s->num_tex_paths; i++)
		scene_debug_texture(s->tex_paths[i], i);
}

