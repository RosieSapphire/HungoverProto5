#include <stdlib.h>
#include <assert.h>

#include "util.h"
#include "scene.h"

void scene_read_mesh(struct mesh *m, const char *mpath)
{
	assert(m);
	assert(mpath);

	FILE *mf = fopen(mpath, "rb");

	if (!mf)
	{
		fprintf(stderr, "Couldn't find mesh from '%s'\n", mpath);
		exit(1);
	}

	fread(m->name, sizeof(char), CONF_NAME_MAX, mf);
	freadflipu16(&m->num_verts, mf);
	freadflipu16(&m->num_indis, mf);
	m->verts = malloc(sizeof(struct vertex) * m->num_verts);
	m->indis = malloc(sizeof(u16) * m->num_indis);
	for (u16 j = 0; j < m->num_verts; j++)
	{
		struct vertex *v = m->verts + j;

		freadflipf32(v->pos + 0, mf);
		freadflipf32(v->pos + 1, mf);
		freadflipf32(v->pos + 2, mf);

		freadflipf32(v->uv + 0, mf);
		freadflipf32(v->uv + 1, mf);

		fread(v->col, sizeof(u8), 4, mf);
	}

	for (u16 j = 0; j < m->num_indis; j++)
		freadflipu16(m->indis + j, mf);

	freadflipu16(&m->tind, mf);
	fclose(mf);
}

void scene_read_node(struct scene *s, struct node *n, FILE *f)
{
	fread(n->mesh_path, 1, CONF_PATH_MAX, f);
	freadflipu16(&n->num_children, f);
	freadflipu16(&n->mesh_ind, f);

	for (u8 i = 0; i < 16; i++)
		freadflipf32((f32 *)n->trans + i, f);

	n->children = malloc(sizeof(struct node) * n->num_children);
	for (u16 i = 0; i < n->num_children; i++)
		scene_read_node(s, n->children + i, f);
}

void scene_read_anim(struct animation *a, FILE *f)
{
	fread(a->name, sizeof(char), CONF_NAME_MAX, f);
	freadflipu16(&a->num_pos, f);
	freadflipu16(&a->num_rot, f);
	freadflipu16(&a->num_sca, f);
	a->pos_keys = malloc(sizeof(struct vec3_key) * a->num_pos);
	a->rot_keys = malloc(sizeof(struct vec4_key) * a->num_rot);
	a->sca_keys = malloc(sizeof(struct vec3_key) * a->num_sca);
	for (u16 j = 0; j < a->num_pos; j++)
	{
		struct vec3_key *poskey = a->pos_keys + j;

		freadflipf32(poskey->val + 0, f);
		freadflipf32(poskey->val + 1, f);
		freadflipf32(poskey->val + 2, f);
	}
	for (u16 j = 0; j < a->num_rot; j++)
	{
		struct vec4_key *rotkey = a->rot_keys + j;

		freadflipf32(rotkey->val + 0, f);
		freadflipf32(rotkey->val + 1, f);
		freadflipf32(rotkey->val + 2, f);
		freadflipf32(rotkey->val + 3, f);
	}
	for (u16 j = 0; j < a->num_sca; j++)
	{
		struct vec3_key *scakey = a->sca_keys + j;

		freadflipf32(scakey->val + 0, f);
		freadflipf32(scakey->val + 1, f);
		freadflipf32(scakey->val + 2, f);
	}

	freadflipu16(&a->length, f);
	freadflipu16(&a->mesh_ind, f);
}

static const struct node *_node_from_mesh_ind(const struct node *n,
					      const u16 mid)
{
	if (mid == n->mesh_ind)
		return (n);

	const struct node *nf;

	for (u16 i = 0; i < n->num_children; i++)
	{
		nf = _node_from_mesh_ind(n->children + i, mid);
		if (nf)
			return (nf);
	}

	return (NULL);
}

/**
 * _scene_read - Test Scene Importing
 * @s: Scene to Import To
 * @path: Path to Import From
 */
void scene_read_file(struct scene *s, const char *path)
{
	FILE *sf = fopen(path, "rb");

	assert(s);
	if (!sf)
	{
		fprintf(stderr, "Couldn't find scene at '%s'\n", path);
		exit(1);
	}

	scene_read_node(s, &s->root_node, sf);
	freadflipu16(&s->num_tex_paths, sf);
	for (u16 i = 0; i < s->num_tex_paths; i++)
		fread(s->tex_paths[i], sizeof(char), TEX_PATH_MAX_LEN, sf);

	freadflipu16(&s->num_meshes, sf);
	assert(s->num_meshes > 0);
	s->meshes = malloc(sizeof(struct mesh) * s->num_meshes);
	for (u16 i = 0; i < s->num_meshes; i++)
	{
		const struct node *n = _node_from_mesh_ind(&s->root_node, i);

		if (!n)
			continue;

		char *path_correct = malloc(strlen("filesystem/") +
					    strlen(n->mesh_path) + 5);

		sprintf(path_correct, "filesystem/%s.mdl", n->mesh_path);
		scene_read_mesh(s->meshes + i, path_correct);
		free(path_correct);
	}
	freadflipu16(&s->num_anims, sf);
	for (u16 i = 0; i < s->num_anims; i++)
		scene_read_anim(s->anims + i, sf);

	fclose(sf);
}

