#include <stdio.h>
#include <libdragon.h>
#include <GL/gl.h>

#include "engine/scene.h"

enum scene_index scene_index = SCENE_TITLE;

static void _scene_read_node(struct scene *s, struct node *n, FILE *f)
{
	fread(n->mesh_path, sizeof(char), CONF_PATH_MAX, f);
	fread(&n->num_children, sizeof(u16), 1, f);
	fread(&n->mesh_ind, sizeof(u16), 1, f);
	fread(n->trans, sizeof(f32), 16, f);
	n->children = malloc(sizeof(struct node) * n->num_children);
	for (u16 i = 0; i < n->num_children; i++)
		_scene_read_node(s, n->children + i, f);
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

	_scene_read_node(s, &s->root_node, sf);

	fread(&s->num_meshes, sizeof(u16), 1, sf);
	s->meshes = malloc(sizeof(struct mesh) * s->num_meshes);

	for (u16 i = 0; i < s->num_meshes; i++)
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

	fclose(sf);
}

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
