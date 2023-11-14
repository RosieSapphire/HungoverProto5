#ifndef _MESH_H_
#define _MESH_H_

#include "../../../include/engine/config.h"
#include "vertex.h"

/**
 * struct mesh - Mesh Structure
 * @name: Mesh Name
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

#endif /* _MESH_H_ */
