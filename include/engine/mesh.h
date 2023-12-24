#ifndef _ENGINE_MESH_H_
#define _ENGINE_MESH_H_

/**
 * @file include/engine/mesh.h
 */

#include <libdragon.h>

#include "engine/config.h"
#include "engine/types.h"
#include "engine/vertex.h"

/**
 * Mesh Flags
 */
enum mesh_flags
{
	MESH_IS_ACTIVE = 0x1, ///< Whether Mesh is Active
};

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
	rspq_block_t *block; ///< RSPQ Block to render static meshes faster
	u8 flags; ///< Flags
	u16 tind; ///< Texture Index
};

void mesh_create_data(struct mesh *m, const char *name, const u16 num_verts,
		      const u16 num_indis, const struct vertex *verts,
		      const u16 *indis);
void mesh_destroy(struct mesh *m);
void mesh_draw(const struct mesh *m);
void mesh_gen_rspqblock(struct mesh *m, const u32 tid);
struct mesh *mesh_get_name(struct mesh *array,
			   const char *name, const u16 num_elem);

#endif /* _ENGINE_MESH_H_ */
