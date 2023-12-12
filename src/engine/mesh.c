#include <string.h>
#include <GL/gl.h>
#include <libdragon.h>

#include "engine/mesh.h"

/**
 * mesh_create_data - Creates Mesh from Data
 * @m: Mesh to Write to
 * @name: Mesh Name
 * @num_verts: Number of Vertices
 * @num_indis: Number of Indices
 * @verts: Vertices Array
 * @indis: Indices Array
 */
void mesh_create_data(struct mesh *m, const char *name, const u16 num_verts,
		      const u16 num_indis, const struct vertex *verts,
		      const u16 *indis)
{
	const u32 verts_size = sizeof(*m->verts) * num_verts;
	const u32 indis_size = sizeof(*m->indis) * num_indis;

	strncpy(m->name, name, CONF_NAME_MAX - 1);
	m->num_verts = num_verts;
	m->num_indis = num_indis;
	m->verts = malloc(verts_size);
	m->indis = malloc(indis_size);
	memcpy(m->verts, verts, verts_size);
	memcpy(m->indis, indis, indis_size);
}

/**
 * mesh_destroy - Destroys Mesh
 * @m: Mesh to Destroy
 */
void mesh_destroy(struct mesh *m)
{
	memset(m->name, 0, CONF_NAME_MAX);
	m->num_verts = m->num_indis = 0;
	free(m->verts);
	free(m->indis);
	m->indis = NULL;
	m->verts = NULL;
	rspq_block_free(m->block);
}

/**
 * mesh_draw - Draws a Mesh
 * @m: Mesh to be Drawn
 * @tid: OpenGL Texture ID
 */
void mesh_draw(const struct mesh *m)
{
	rspq_block_run(m->block);
}

/**
 * mesh_gen_rspqblock - Generates RSPQ Block for Mesh (Good for Static Meshes)
 * @m: Mesh to have Block generated for
 * @tid: Texture ID to generate with
 */
void mesh_gen_rspqblock(struct mesh *m, const u32 tid)
{
	rspq_block_begin();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex), m->verts->pos);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex), m->verts->uv);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE,
		sizeof(struct vertex), m->verts->col);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tid);

	glDrawElements(GL_TRIANGLES, m->num_indis,
		GL_UNSIGNED_SHORT, m->indis);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	m->block = rspq_block_end();
}

/**
 * mesh_get_name - Get a Mesh by Name
 * @array: Array of Meshes to search through
 * @name: Name you are looking for
 * @num_elem: Count of Array Elements
 *
 * Return: Desired Mesh or NULL
 */
struct mesh *mesh_get_name(struct mesh *array,
			   const char *name, const u16 num_elem)
{
	for (u16 i = 0; i < num_elem; i++)
	{
		struct mesh *m = array + i;

		if (!strcmp(m->name, name))
			return (m);
	}

	return (NULL);
}
