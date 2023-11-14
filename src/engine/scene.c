#include <stdio.h>
#include <libdragon.h>

#include "engine/scene.h"

enum scene_index scene_index = SCENE_TITLE;

void scene_read_file(struct scene *s, const char *path)
{
	FILE *f = fopen(path, "rb");

	if (!f)
	{
		debugf("Couldn't find scene at '%s'\n", path);
		exit(1);
	}

	fread(&s->num_meshes, sizeof(u16), 1, f);
	s->meshes = malloc(sizeof(struct mesh) * s->num_meshes);
	for (u16 i = 0; i < s->num_meshes; i++)
	{
		struct mesh *m = s->meshes + i;

		fread(m->name, sizeof(char), CONF_NAME_MAX, f);
		fread(&m->num_verts, sizeof(u16), 1, f);
		fread(&m->num_indis, sizeof(u16), 1, f);
		m->verts = malloc(sizeof(struct vertex) * m->num_verts);
		m->indis = malloc(sizeof(u16) * m->num_indis);
		for (u16 j = 0; j < m->num_verts; j++)
		{
			struct vertex *v = m->verts + j;

			fread(v->pos, sizeof(f32), 3, f);
			fread(v->uv, sizeof(f32), 2, f);
			fread(v->col, sizeof(u32), 1, f);
		}

		fread(m->indis, sizeof(u16), m->num_indis, f);
	}

	fclose(f);
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
