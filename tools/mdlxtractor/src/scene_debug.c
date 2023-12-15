#include <stdio.h>

#include "scene.h"

void scene_debug_node(const struct node *n, const u8 depth)
{
	for (u8 i = 0; i < depth; i++)
		printf("\t");
	printf("[NODE %d] '%s'\n", n->mesh_ind, n->mesh_path);
	for (u16 i = 0; i < n->num_children; i++)
		scene_debug_node(n->children + i, depth + 1);
}

void scene_debug_texture(const char *tpath, const u16 ind)
{
	printf("[TEXTURE %d] '%s'\n", ind, tpath);
}

void scene_debug_anim_keys(const struct animation *a)
{
	for (u16 j = 0; j < a->num_pos; j++)
	{
		const f32 *pos = a->pos_keys[j].val;

		printf("\t[POS %d] %.3f, %.3f, %.3f\n",
		       j, pos[0], pos[1], pos[2]);
	}

	printf("\n");

	for (u16 j = 0; j < a->num_rot; j++)
	{
		const f32 *rot = a->rot_keys[j].val;

		printf("\t[ROTATE %d] %.3f, %.3f, %.3f, %.3f\n",
		       j, rot[0], rot[1], rot[2], rot[3]);
	}

	printf("\n");

	for (u16 j = 0; j < a->num_sca; j++)
	{
		const f32 *sca = a->sca_keys[j].val;

		printf("\t[SCALE %d] %.3f, %.3f, %.3f\n",
		       j, sca[0], sca[1], sca[2]);
	}
}

/**
 * scene_debug_assimp - Debugs everything for Scene Structure
 * @s: Scene to Debug
 */
void scene_debug_assimp(const struct scene *s)
{
	printf("[SCENE]\n");
	for (u16 i = 0; i < s->num_meshes; i++)
	{
		const struct mesh *m = s->meshes + i;

		printf("\t[MESH %d] %s\n", i, m->name);
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
	}

	scene_debug_node(&s->root_node, 0);

	printf("\n");

	for (u16 i = 0; i < s->num_anims; i++)
	{
		const struct animation *a = s->anims + i;

		printf("[ANIM %d] %s\n", i, a->name);
		scene_debug_anim_keys(a);
	}

	for (u16 i = 0; i < s->num_tex_paths; i++)
		scene_debug_texture(s->tex_paths[i], i);
}

