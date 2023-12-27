#ifndef _ENGINE_COLLISSION_MESH_H_
#define _ENGINE_COLLISSION_MESH_H_

/**
 * @file include/engine/collision_mesh.h
 */

#include "engine/mesh.h"

/**
 * Vector 3 Typedef
 */
typedef f32 vec3[3];

/**
 * Enum for Collision Type
 */
enum collision_type
{
	COLTYPE_FLOOR, ///< Floor Collision Type
	COLTYPE_WALLS, ///< Walls Collision Type
	COLTYPE_COUNT, ///< Number of Collision Types
};

/**
 * Collision Mesh Structure
 */
struct collision_mesh
{
	u16 num_verts; ///< Number of vertices
	vec3 * verts; ///< Vertices array
};

/**
 * @defgroup engine Engine Functions
 * @{
 * @defgroup collision_mesh Collision Mesh Functions
 * @{
 */
void collision_mesh_gen(struct collision_mesh *out,
			const struct mesh *in);
void collision_mesh_free(struct collision_mesh *m);
/**
 * @}
 * @}
 */

#endif /* _ENGINE_COLLISSION_MESH_H_ */
