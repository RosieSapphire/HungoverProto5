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
 * @tind: Texture Index for Scene
 */
struct mesh
{
	char name[CONF_NAME_MAX];
	u16 num_verts, num_indis;
	struct vertex *verts;
	u16 *indis;
	u16 tind;
};

void mesh_path_from_scene_path(char *out, const char *mname,
			       const char *scnpath);
u16 mesh_ind_by_name(const char *name, const struct mesh *meshes,
		     const u16 num_meshes);

#endif /* _MESH_H_ */
