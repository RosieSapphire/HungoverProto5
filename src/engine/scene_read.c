/**
 * @file src/engine/scene_read.c
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <libdragon.h>
#include <GL/gl.h>

#include "engine/texture.h"
#include "engine/scene.h"

static void _scene_read_node(struct node *n, FILE *f)
{
	fread(n->mesh_path, sizeof(char), CONF_PATH_MAX, f);
	fread(&n->num_children, sizeof(u16), 1, f);
	fread(&n->mesh_ind, sizeof(u16), 1, f);
	fread(n->trans, sizeof(f32), 16, f);
	n->children = malloc(sizeof(struct node) * n->num_children);
	for (u16 i = 0; i < n->num_children; i++)
		_scene_read_node(n->children + i, f);
}

static void _scene_read_mesh(struct scene *s, u16 i)
{
	const char *prefix = "rom:/";
	struct node *n = node_from_mesh_ind(&s->root_node, i);
	char *mfpath_full = malloc(strlen(n->mesh_path) + 5);

	sprintf(mfpath_full, "%s.mdl", n->mesh_path);
	char *mfpath_conv = malloc(strlen(mfpath_full) + strlen(prefix));

	strcpy(mfpath_conv, prefix);
	strcat(mfpath_conv, mfpath_full);
	free(mfpath_full);
	FILE *mf = fopen(mfpath_conv, "rb");

	assertf(mf, "Couldn't find mesh at '%s'\n", mfpath_conv);
	free(mfpath_conv);
	struct mesh *m = s->meshes + i;

	fread(m->name, sizeof(char), CONF_NAME_MAX, mf);
	fread(&m->num_verts, sizeof(u16), 1, mf);
	fread(&m->num_indis, sizeof(u16), 1, mf);
	m->verts = malloc(sizeof(struct vertex) * m->num_verts);
	m->indis = malloc(sizeof(u16) * m->num_indis);
	for (u16 j = 0; j < m->num_verts; j++)
	{
		struct vertex *v = m->verts + j;

		fread(v->pos, sizeof(f32), 3, mf);
		fread(v->uv, sizeof(f32), 2, mf);
		fread(v->col, sizeof(u32), 1, mf);
	}
	fread(m->indis, sizeof(u16), m->num_indis, mf);
	fread(&m->tind, sizeof(u16), 1, mf);
	fclose(mf);
	const u32 id = m->tind == 0xFFFF ? 0 : s->tex_ids[m->tind];

	mesh_gen_rspqblock(m, id);
	m->flags = MESH_IS_ACTIVE;
}

static void _scene_read_animation(struct animation *a, FILE *f)
{
	fread(a->name, sizeof(char), CONF_NAME_MAX, f);
	fread(&a->num_pos, sizeof(u16), 1, f);
	fread(&a->num_rot, sizeof(u16), 1, f);
	fread(&a->num_sca, sizeof(u16), 1, f);
	a->pos_keys = malloc(sizeof(struct vec3_key) * a->num_pos);
	a->rot_keys = malloc(sizeof(struct vec4_key) * a->num_rot);
	a->sca_keys = malloc(sizeof(struct vec3_key) * a->num_sca);
	for (u16 j = 0; j < a->num_pos; j++)
		fread(a->pos_keys + j, sizeof(f32), 3, f);

	for (u16 j = 0; j < a->num_rot; j++)
		fread(a->rot_keys + j, sizeof(f32), 4, f);

	for (u16 j = 0; j < a->num_sca; j++)
		fread(a->sca_keys + j, sizeof(f32), 3, f);

	fread(&a->length, sizeof(u16), 1, f);
	fread(&a->mesh_ind, sizeof(u16), 1, f);
	a->frame = 0;
	a->frame_last = 0;
	a->flags = ANIM_IS_PLAYING | ANIM_LOOPS;
}

/**
 * Reads Scene Object from File
 * @param[out] s Scene Read Output
 * @param[in] path Path to File to Read from
 */
void scene_read_file(struct scene *s, const char *path)
{
	FILE *sf = fopen(path, "rb");

	assertf(s, "Scene output pointer is NULL.\n");
	assertf(sf, "Couldn't find scene at '%s'.\n", path);
	_scene_read_node(&s->root_node, sf);

	fread(&s->num_tex_ids, sizeof(u16), 1, sf);
	s->tex_ids = malloc(sizeof(u32) * s->num_tex_ids);
	for (u16 i = 0; i < s->num_tex_ids; i++)
	{
		char tpath[TEX_PATH_MAX_LEN];

		fread(tpath, sizeof(char), TEX_PATH_MAX_LEN, sf);
		s->tex_ids[i] = texture_create_file(tpath);
	}

	fread(&s->num_meshes, sizeof(u16), 1, sf);
	s->meshes = malloc(sizeof(struct mesh) * s->num_meshes);
	for (u16 i = 0; i < s->num_meshes; i++)
		_scene_read_mesh(s, i);

	fread(&s->num_anims, sizeof(u16), 1, sf);
	s->anims = malloc(sizeof(struct animation) * s->num_anims);
	for (u16 i = 0; i < s->num_anims; i++)
		_scene_read_animation(s->anims + i, sf);

	fclose(sf);
}
