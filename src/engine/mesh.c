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
