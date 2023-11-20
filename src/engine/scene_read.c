#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <libdragon.h>

#include "engine/scene.h"

/**
 * _scene_read_node - Reads a Node from a Scene file
 * @n: Node to Read to
 * @f: File to Read from
 */
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

/**
 * _scene_read_mesh - Reads a Mesh from its own File
 * @s: Scene to Read Mesh to
 * @i: Mesh Index
 */
static void _scene_read_mesh(const struct scene *s, u16 i)
{
	const char *prefix = "rom:/";
	const char *mfpath = s->root_node.children[i].mesh_path;
	char *mfpath_conv = malloc(strlen(mfpath) + strlen(prefix));

	strcpy(mfpath_conv, prefix);
	strcat(mfpath_conv, mfpath + strlen("assets/"));

	FILE *mf = fopen(mfpath_conv, "rb");

	if (!mf)
	{
		debugf("Couldn't find mesh at '%s'\n", mfpath_conv);
		exit(1);
	}

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
	fclose(mf);
}

/**
 * _scene_read_animation - Reads an Animation from a Scene Struct
 */
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
	a->frame = a->frame_last = 0;
	a->flags = ANIM_IS_PLAYING | ANIM_LOOPS;
}

/**
 * scene_read_file - Reads Scene Object from File
 * @s: Scene Read Output
 * @path: Path to File to Read from
 */
void scene_read_file(struct scene *s, const char *path)
{
	FILE *sf = fopen(path, "rb");

	if (!sf)
	{
		debugf("Couldn't find scene at '%s'\n", path);
		exit(1);
	}

	_scene_read_node(&s->root_node, sf);

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
