#include <stdio.h>
#include <libdragon.h>

#include "engine/scene.h"

enum scene_index scene_index = SCENE_TITLE;

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

	fread(&s->num_meshes, sizeof(u16), 1, sf);
	s->meshes = malloc(sizeof(struct mesh) * s->num_meshes);

	const char *prefix = "rom:/";
	char mfpath[CONF_NAME_MAX];

	fread(mfpath, sizeof(char), CONF_NAME_MAX, sf);

	char *mfpath_conv = malloc(strlen(mfpath) + strlen(prefix));

	strcpy(mfpath_conv, prefix);
	strcat(mfpath_conv, mfpath + strlen("filesystem/"));

	FILE *mf = fopen(mfpath_conv, "rb");

	if (!mf)
	{
		debugf("Couldn't find mesh at '%s'\n", mfpath_conv);
		exit(1);
	}

	free(mfpath_conv);

	for (u16 i = 0; i < s->num_meshes; i++)
	{
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
	}

	fclose(mf);
	fclose(sf);
}

/**
 * scene_draw - Draws a Scene and its Meshes
 * @s: Scene to Draw
 */
void scene_draw(const struct scene *s)
{
	for (u16 i = 0; i < s->num_meshes; i++)
		mesh_draw(s->meshes + i, 0);
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
