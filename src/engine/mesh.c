#include <string.h>
#include <libdragon.h>

#include "engine/mesh.h"

/**
 * _mesh_debug - Debugs data from a Mesh
 * @m: Mesh to Debug
 */
static void _mesh_debug(const struct mesh *m)
{
	debugf("[MESH] name=%s num_verts=%u num_indis=%u\n",
	m->name, m->num_verts, m->num_indis);
	for (u16 i = 0; i < m->num_verts; i++)
	{
		const struct vertex *v = m->verts + i;

		debugf("\t[VERT %u] %f, %f, %f\n", i,
			v->pos[0], v->pos[1], v->pos[2]);
	}
}

/**
 * mesh_create_data - Creates Mesh from Data
 * @name: Mesh Name
 * @num_verts: Number of Vertices
 * @num_indis: Number of Indices
 * @verts: Vertices Array
 * @indis: Indices Array
 *
 * Return: Constructed Mesh
 */
struct mesh *mesh_create_data(const char *name, const u16 num_verts,
			      const u16 num_indis, const struct vertex *verts,
			      const u16 *indis)
{
	struct mesh *m = malloc(sizeof(*m));
	const u32 verts_size = sizeof(*m->verts) * num_verts;
	const u32 indis_size = sizeof(*m->indis) * num_indis;

	strncpy(m->name, name, CONF_NAME_MAX - 1);
	m->num_verts = num_verts;
	m->num_indis = num_indis;
	m->verts = malloc(verts_size);
	m->indis = malloc(indis_size);
	memcpy(m->verts, verts, verts_size);
	memcpy(m->indis, indis, indis_size);
	_mesh_debug(m);

	return (m);
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
}
