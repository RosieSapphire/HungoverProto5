#ifndef _ENGINE_MESH_H_
#define _ENGINE_MESH_H_

#include "engine/config.h"
#include "engine/types.h"
#include "engine/vertex.h"

/**
 * struct mesh - Mesh Structure
 * @num_verts: Number of Vertices
 * @num_indis: Number of Indices
 * @verts: Vertices Array
 * @indis: Indices Array
 */
struct mesh
{
	char name[CONF_NAME_MAX];
	u16 num_verts, num_indis;
	struct vertex *verts;
	u16 *indis;
};

struct mesh *mesh_create_data(const char *name, const u16 num_verts,
			      const u16 num_indis, const struct vertex *verts,
			      const u16 *indis);

#endif /* _ENGINE_MESH_H_ */
