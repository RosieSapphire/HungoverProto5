/**
 * @file include/mesh.h
 */

#ifndef _MESH_H_
#define _MESH_H_

#include "../../../include/engine/config.h"
#include "vertex.h"

/**
 * Mesh Structure
 */
struct mesh
{
	char name[CONF_NAME_MAX]; ///< Mesh Name
	u16 num_verts; ///< Number of Vertices
	u16 num_indis; ///< Number of Indices
	struct vertex *verts; ///< Vertices Array
	u16 *indis; ///< Indices Array
	u16 tind; ///< Texture Index for Scene
};

/**
 * Get Mesh Path from Scene Path
 * @param[out] out Output String
 * @param[in] mname Mesh Name
 * @param[in] scnpath Scene Path
 */
void mesh_path_from_scene_path(char *out, const char *mname,
			       const char *scnpath);

/**
 * Get Mesh Index by Name
 * @param[in] name Name of Desired Mesh
 * @param[in] meshes Meshes array
 * @param[in] num_meshes Number of Meshes
 * @return Mesh Index from Name
 */
u16 mesh_ind_by_name(const char *name, const struct mesh *meshes,
		     const u16 num_meshes);

#endif /* _MESH_H_ */
